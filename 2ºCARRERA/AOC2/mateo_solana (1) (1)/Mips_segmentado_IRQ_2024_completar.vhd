----------------------------------------------------------------------------------
-- Authors: Mateo Lorente, Diego  --- 873338
--			Solana Melero, Carlos --- 872815
-- Description: Mips segmentado tal y como lo hemos estudiado en clase con:
-- Unidad de anticipaci�n (incompleta)
-- Unidad de detenci�n (incompleta)
-- Unidad de control (incompleta)
-- Saltos en ID
-- instrucciones aritm�ticas, LW, SW, NOP y BEQ
-- MI y MD de 128 palabras de 32 bits
-- Gesti�n de excepciones: IRQ, ABORT y UNDEF (incompleta)
-- L�nea de IRQ
-- Nuevas instrucciones: RTE, JAL y RET (incompleta)
-- Hay funcionalidad incompleta en este archivo, y en UC, UD, EM y UA. Buscar la etiqueta: completar
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity MIPs_segmentado is
    Port ( 	clk : in  STD_LOGIC;
           	reset : in  STD_LOGIC;
           	-- Interface with the IO/MD subsystem
           	ADDR : OUT std_logic_vector (31 downto 0); --Dir solicitada por el Mips
          	Dout : out std_logic_vector (31 downto 0); -- Datos que envia el Mips al subsistema de I/O y MD
           	Din : in std_logic_vector (31 downto 0);-- Datos que recibe el Mips del subsistema de I/O y MD
		 	WE : OUT std_logic;		-- write enable	del MIPS
		  	RE : OUT std_logic;		-- read enable del MIPS			  
		  	Mem_ready: in std_logic; -- Nos avisa de si el subsistema de IO/MD va a realizar en este ciclo la orden del MIPS
		  	-- Exceptions
		  	IRQ	: 	in  STD_LOGIC; 
		  	Data_abort: in std_logic --indica que el �ltimo acceso a memoria ha sido un error
		  	);
end MIPs_segmentado;

architecture Behavioral of MIPs_segmentado is

component reg is
    generic (size: natural := 32);  -- por defecto son de 32 bits, pero se puede usar cualquier tama�o
	Port ( Din : in  STD_LOGIC_VECTOR (size -1 downto 0);
           clk : in  STD_LOGIC;
		   reset : in  STD_LOGIC;
           load : in  STD_LOGIC;
           Dout : out  STD_LOGIC_VECTOR (size -1 downto 0));
end component;
---------------------------------------------------------------

component adder32 is
    Port ( Din0 : in  STD_LOGIC_VECTOR (31 downto 0);
           Din1 : in  STD_LOGIC_VECTOR (31 downto 0);
           Dout : out  STD_LOGIC_VECTOR (31 downto 0));
end component;

component mux4_1 is
  Port (   DIn0 : in  STD_LOGIC_VECTOR (31 downto 0);
           DIn1 : in  STD_LOGIC_VECTOR (31 downto 0);
           DIn2 : in  STD_LOGIC_VECTOR (31 downto 0);
           DIn3 : in  STD_LOGIC_VECTOR (31 downto 0);
		   ctrl : in  STD_LOGIC_VECTOR (1 downto 0);
           Dout : out  STD_LOGIC_VECTOR (31 downto 0));
end component;

component mux2_1 is
  Port (   DIn0 : in  STD_LOGIC_VECTOR (31 downto 0);
           DIn1 : in  STD_LOGIC_VECTOR (31 downto 0);
		   ctrl : in  STD_LOGIC;
           Dout : out  STD_LOGIC_VECTOR (31 downto 0));
end component;

component memoriaRAM_I is port (
		  CLK : in std_logic;
		  ADDR : in std_logic_vector (31 downto 0); --Dir 
          Din : in std_logic_vector (31 downto 0);--entrada de datos para el puerto de escritura
          WE : in std_logic;		-- write enable	
		  RE : in std_logic;		-- read enable		  
		  Dout : out std_logic_vector (31 downto 0));
end component;

component Banco_ID is
 Port ( IR_in : in  STD_LOGIC_VECTOR (31 downto 0); -- instrucci�n leida en IF
        PC4_in:  in  STD_LOGIC_VECTOR (31 downto 0); -- PC+4 sumado en IF
		clk : in  STD_LOGIC;
		reset : in  STD_LOGIC;
        load : in  STD_LOGIC;
        IR_ID : out  STD_LOGIC_VECTOR (31 downto 0); -- instrucci�n en la etapa ID
        PC4_ID:  out  STD_LOGIC_VECTOR (31 downto 0);
        --nuevo para excepciones
        PC_exception:  in  STD_LOGIC_VECTOR (31 downto 0); -- PC al que se volver� si justo esta instrucci�n est� en MEM cuando llega una excepci�n. 
        PC_exception_ID:  out  STD_LOGIC_VECTOR (31 downto 0);-- PC+4 en la etapa ID
        --bits de validez
        valid_I_IF: in STD_LOGIC;
        valid_I_ID: out STD_LOGIC ); 
end component;

COMPONENT BReg
    PORT(
         clk : IN  std_logic;
		 reset : in  STD_LOGIC;
         RA : IN  std_logic_vector(4 downto 0);
         RB : IN  std_logic_vector(4 downto 0);
         RW : IN  std_logic_vector(4 downto 0);
         BusW : IN  std_logic_vector(31 downto 0);
         RegWrite : IN  std_logic;
         BusA : OUT  std_logic_vector(31 downto 0);
         BusB : OUT  std_logic_vector(31 downto 0)
        );
END COMPONENT;

component Ext_signo is
    Port ( inm : in  STD_LOGIC_VECTOR (15 downto 0);
           inm_ext : out  STD_LOGIC_VECTOR (31 downto 0));
end component;

component two_bits_shifter is
    Port ( Din : in  STD_LOGIC_VECTOR (31 downto 0);
           Dout : out  STD_LOGIC_VECTOR (31 downto 0));
end component;

component UC is
Port ( 		valid_I_ID : in  STD_LOGIC; --indica si es una instrucci�n v�lida			
			IR_op_code : in  STD_LOGIC_VECTOR (5 downto 0);
         	Branch : out  STD_LOGIC;
           	RegDst : out  STD_LOGIC;
           	ALUSrc : out  STD_LOGIC;
		   	MemWrite : out  STD_LOGIC;
           	MemRead : out  STD_LOGIC;
           	MemtoReg : out  STD_LOGIC;
           	RegWrite : out  STD_LOGIC;
          	jal : out  STD_LOGIC; --indica que es una instrucci�n jal 
        	ret : out  STD_LOGIC; --indica que es una instrucci�n ret
			undef: out STD_LOGIC; --indica que el c�digo de operaci�n no pertenence a una instrucci�n conocida. En este procesador se usa s�lo para depurar
           	 -- Nuevas se�ales
		   	RTE	: out  STD_LOGIC -- indica que es una instrucci�n RTE	   
			  -- Fin Nuevas se�ales
           );
end component;
-- NUEVO
-- Unidad de detecci�n de riesgos
component UD is
Port (   	valid_I_ID : in  STD_LOGIC; --indica si es una instrucci�n v�lida
			valid_I_EX : in  STD_LOGIC; --indica si es una instrucci�n de EX es v�lida
			valid_I_MEM : in  STD_LOGIC; --indica si es una instrucci�n de MEM es v�lida
			Reg_Rs_ID: in  STD_LOGIC_VECTOR (4 downto 0); --registros Rs y Rt en la etapa ID
		  	Reg_Rt_ID	: in  STD_LOGIC_VECTOR (4 downto 0);
			MemRead_EX	: in std_logic; -- información sobre la instrucci�n en EX (destino, si lee de memoria y si escribe en registro)
			RegWrite_EX	: in std_logic;
			RW_EX			: in  STD_LOGIC_VECTOR (4 downto 0);
			RegWrite_Mem	: in std_logic;-- informacion sobre la instruccion en Mem (destino y si escribe en registro)
			RW_Mem			: in  STD_LOGIC_VECTOR (4 downto 0);
			IR_op_code	: in  STD_LOGIC_VECTOR (5 downto 0); -- c�digo de operaci�n de la instrucci�n en IEEE
         	salto_tomado			: in std_logic; -- 1 cuando se produce un salto 0 en caso contrario
         	--Nuevo
         	JAL_EX : in std_logic; -- Indica que la instrucci�n en EX es un JAL
         	JAL_MEM : in std_logic; -- Indica que la instrucci�n en MEM es un JAL
         	Mem_ready: in std_logic; -- 1 cuando la memoria puede realizar la operaci�n solicitada en el ciclo actual
			parar_MIPS: out  STD_LOGIC; -- Indica que todas las etapas deben parar
			Kill_IF		: out  STD_LOGIC; -- Indica que la instrucci�n en IF no debe ejecutarse (fallo en la predicci�n de salto tomado)
			Parar_ID		: out  STD_LOGIC); -- Indica que las etapas ID y previas deben parar
end component;

COMPONENT Banco_EX
    PORT(
         	 clk : in  STD_LOGIC;
			reset : in  STD_LOGIC;
			load : in  STD_LOGIC;
	        busA : in  STD_LOGIC_VECTOR (31 downto 0);
           	busB : in  STD_LOGIC_VECTOR (31 downto 0);
			busA_EX : out  STD_LOGIC_VECTOR (31 downto 0);
           	busB_EX : out  STD_LOGIC_VECTOR (31 downto 0);
           	RegDst_ID : in  STD_LOGIC;
           	ALUSrc_ID : in  STD_LOGIC;
           	MemWrite_ID : in  STD_LOGIC;
           	MemRead_ID : in  STD_LOGIC;
           	MemtoReg_ID : in  STD_LOGIC;
           	RegWrite_ID : in  STD_LOGIC;
			inm_ext: IN  std_logic_vector(31 downto 0);
			inm_ext_EX: OUT  std_logic_vector(31 downto 0);
           	RegDst_EX : out  STD_LOGIC;
           	ALUSrc_EX : out  STD_LOGIC;
           	MemWrite_EX : out  STD_LOGIC;
           	MemRead_EX : out  STD_LOGIC;
           	MemtoReg_EX : out  STD_LOGIC;
           	RegWrite_EX : out  STD_LOGIC;
			-- Nuevo: para la unidad de anticipaci�n
			Reg_Rs_ID : in  std_logic_vector(4 downto 0);
			Reg_Rs_EX : out std_logic_vector(4 downto 0);
			--Fin nuevo
			ALUctrl_ID: in STD_LOGIC_VECTOR (2 downto 0);
			ALUctrl_EX: out STD_LOGIC_VECTOR (2 downto 0);
           	Reg_Rt_ID : in  STD_LOGIC_VECTOR (4 downto 0);
           	Reg_Rd_ID : in  STD_LOGIC_VECTOR (4 downto 0);
           	Reg_Rt_EX : out  STD_LOGIC_VECTOR (4 downto 0);
           	Reg_Rd_EX : out  STD_LOGIC_VECTOR (4 downto 0);
            -- Nuevo excepci�n
           	PC_exception_ID:  in  STD_LOGIC_VECTOR (31 downto 0);
           	PC_exception_EX:  out  STD_LOGIC_VECTOR (31 downto 0);
           	-- Nuevo para el retorno de la excepci�n
           	RTE_ID :  in STD_LOGIC; 
           	RTE_EX :  out STD_LOGIC; 
           	--bits de validez
        	valid_I_EX_in: in STD_LOGIC;
        	valid_I_EX: out STD_LOGIC;
        	-- Puertos para a�adir se�ales
			-- Estos puertos se utilizan para a�adir funcionalidades al MIPS que requieran enviar informaci�n de una etapa a las etapas siguientes
			-- El banco permite enviar dos se�ales de un bit (ext_signal_1 y 2) y dos palabras de 32 bits ext_word_1 y 2)
			ext_signal_1_ID: in  STD_LOGIC;
			ext_signal_1_EX: out  STD_LOGIC;
			ext_signal_2_ID: in  STD_LOGIC;
			ext_signal_2_EX: out  STD_LOGIC;
			ext_word_1_ID:  IN  STD_LOGIC_VECTOR (31 downto 0);
			ext_word_1_EX:  OUT  STD_LOGIC_VECTOR (31 downto 0);
			ext_word_2_ID:  IN  STD_LOGIC_VECTOR (31 downto 0);
			ext_word_2_EX:  OUT  STD_LOGIC_VECTOR (31 downto 0)
			--fin puertos extensi�n
			);
    END COMPONENT;
-- NUEVO        
-- Unidad de anticipaci�n de operandos
    COMPONENT UA
	Port(
			valid_I_MEM : in  STD_LOGIC; --indica si es una instrucci�n de MEM es v�lida
			valid_I_WB : in  STD_LOGIC; --indica si es una instrucci�n de WB es v�lida
			Reg_Rs_EX: IN  std_logic_vector(4 downto 0); 
			Reg_Rt_EX: IN  std_logic_vector(4 downto 0);
			RegWrite_MEM: IN std_logic;
			RW_MEM: IN  std_logic_vector(4 downto 0);
			RegWrite_WB: IN std_logic;
			RW_WB: IN  std_logic_vector(4 downto 0);
			MUX_ctrl_A: out std_logic_vector(1 downto 0);
			MUX_ctrl_B: out std_logic_vector(1 downto 0)
		);
	end component;

	
	COMPONENT ALU
    PORT(
         DA : IN  std_logic_vector(31 downto 0);
         DB : IN  std_logic_vector(31 downto 0);
         ALUctrl : IN  std_logic_vector(2 downto 0);
         Dout : OUT  std_logic_vector(31 downto 0)
               );
    END COMPONENT;
	 
	component mux2_5bits is
	Port ( DIn0 : in  STD_LOGIC_VECTOR (4 downto 0);
		   DIn1 : in  STD_LOGIC_VECTOR (4 downto 0);
		   ctrl : in  STD_LOGIC;
		   Dout : out  STD_LOGIC_VECTOR (4 downto 0));
	end component;
	
COMPONENT Banco_MEM
    PORT(
       		ALU_out_EX : in  STD_LOGIC_VECTOR (31 downto 0); 
			ALU_out_MEM : out  STD_LOGIC_VECTOR (31 downto 0); -- instrucci�n leida en IF
         	clk : in  STD_LOGIC;
			reset : in  STD_LOGIC;
    		load : in  STD_LOGIC;
			MemWrite_EX : in  STD_LOGIC;
    	    MemRead_EX : in  STD_LOGIC;
        	MemtoReg_EX : in  STD_LOGIC;
         	RegWrite_EX : in  STD_LOGIC;
			MemWrite_MEM : out  STD_LOGIC;
        	MemRead_MEM : out  STD_LOGIC;
         	MemtoReg_MEM : out  STD_LOGIC;
         	RegWrite_MEM : out  STD_LOGIC;
         	BusB_EX: in  STD_LOGIC_VECTOR (31 downto 0); -- para los store
			BusB_MEM: out  STD_LOGIC_VECTOR (31 downto 0); -- para los store
			RW_EX : in  STD_LOGIC_VECTOR (4 downto 0); -- registro destino de la escritura
         	RW_MEM : out  STD_LOGIC_VECTOR (4 downto 0);
         	-- Nuevo excepci�n
            PC_exception_EX:  in  STD_LOGIC_VECTOR (31 downto 0);
            PC_exception_MEM:  out  STD_LOGIC_VECTOR (31 downto 0);           	
         	--bits de validez
        	valid_I_EX: in STD_LOGIC;
        	valid_I_MEM: out STD_LOGIC;
        	-- Puertos para a�adir se�ales
			-- Estos puertos se utilizan para a�adir funcionalidades al MIPS que requieran enviar informaci�n de una etapa a las etapas siguientes
			-- El banco permite enviar dos se�ales de un bit (ext_signal_1 y 2) y dos palabras de 32 bits ext_word_1 y 2)
			ext_signal_1_EX: in  STD_LOGIC;
			ext_signal_1_MEM: out  STD_LOGIC;
			ext_signal_2_EX: in  STD_LOGIC;
			ext_signal_2_MEM: out  STD_LOGIC;
			ext_word_1_EX:  IN  STD_LOGIC_VECTOR (31 downto 0);
			ext_word_1_MEM:  OUT  STD_LOGIC_VECTOR (31 downto 0);
			ext_word_2_EX:  IN  STD_LOGIC_VECTOR (31 downto 0);
			ext_word_2_MEM:  OUT  STD_LOGIC_VECTOR (31 downto 0)
			--fin puertos extensi�n
	);
END COMPONENT;
 
    COMPONENT Banco_WB
    PORT(
        ALU_out_MEM : in  STD_LOGIC_VECTOR (31 downto 0); 
		ALU_out_WB : out  STD_LOGIC_VECTOR (31 downto 0); 
		MEM_out : in  STD_LOGIC_VECTOR (31 downto 0); 
		MDR : out  STD_LOGIC_VECTOR (31 downto 0); --memory data register
        clk : in  STD_LOGIC;
		reset : in  STD_LOGIC;
        load : in  STD_LOGIC;
		MemtoReg_MEM : in  STD_LOGIC;
        RegWrite_MEM : in  STD_LOGIC;
		MemtoReg_WB : out  STD_LOGIC;
        RegWrite_WB : out  STD_LOGIC;
        RW_MEM : in  STD_LOGIC_VECTOR (4 downto 0); -- registro destino de la escritura
        RW_WB : out  STD_LOGIC_VECTOR (4 downto 0); -- PC+4 en la etapa IDend Banco_WB;
        --bits de validez
        valid_I_WB_in: in STD_LOGIC;
        valid_I_WB: out STD_LOGIC;
        -- Puertos para a�adir se�ales
		-- Estos puertos se utilizan para a�adir funcionalidades al MIPS que requieran enviar informaci�n de una etapa a las etapas siguientes
		-- El banco permite enviar dos se�ales de un bit (ext_signal_1 y 2) y dos palabras de 32 bits ext_word_1 y 2)
		ext_signal_1_MEM: in  STD_LOGIC;
		ext_signal_1_WB: out  STD_LOGIC;
		ext_signal_2_MEM: in  STD_LOGIC;
		ext_signal_2_WB: out  STD_LOGIC;
		ext_word_1_MEM:  IN  STD_LOGIC_VECTOR (31 downto 0);
		ext_word_1_WB:  OUT  STD_LOGIC_VECTOR (31 downto 0);
		ext_word_2_MEM:  IN  STD_LOGIC_VECTOR (31 downto 0);
		ext_word_2_WB:  OUT  STD_LOGIC_VECTOR (31 downto 0)
		--fin puertos extensi�n
		);
    END COMPONENT; 
    
    COMPONENT counter 
 	generic (
   			size : integer := 10);
	Port ( 	clk : in  STD_LOGIC;
       		reset : in  STD_LOGIC;
       		count_enable : in  STD_LOGIC;
       		count : out  STD_LOGIC_VECTOR (size-1 downto 0));
	end COMPONENT;
	
	COMPONENT Exception_manager is
    Port ( 	clk : in  STD_LOGIC;
           	reset : in  STD_LOGIC;
           	IRQ	: 	in  STD_LOGIC; 
           	Data_abort: in std_logic; --indica que el �ltimo acceso a memoria ha sido un error
           	undef: in STD_LOGIC; --indica que el c�digo de operaci�n no pertenence a una instrucci�n conocida. En este procesador se usa s�lo para depurar
           	RTE_ID: in STD_LOGIC; -- indica que en ID hay una instrucci�n de retorno de Excepci�n v�lida
           	RTE_EX: in STD_LOGIC; -- indica que en EX hay una instrucci�n de retorno de Excepci�n v�lida
           	valid_I_ID: in STD_LOGIC; -- indica que la instrucci�n en ID es v�lida
           	valid_I_EX: in STD_LOGIC; -- indica que la instrucci�n en EX es v�lida
           	valid_I_MEM: in STD_LOGIC; -- indica que la instrucci�n en MEM es v�lida
           	parar_MIPS: in STD_LOGIC; -- indica que hay que detener todas las etapas del mips
           	PC_out: std_logic_vector(31 downto 0);-- pc actual
           	PC_exception_EX: std_logic_vector(31 downto 0); --PC de la Ins en EX
           	PC_exception_ID: std_logic_vector(31 downto 0); --PC de la Ins en ID
           	Exception_accepted: out STD_LOGIC; -- Informa que se va a ceptar un excepci�n en el ciclo actual
           	Exception_LR_output: out std_logic_vector(31 downto 0)
           	);         	
	end COMPONENT;
--------------------------------------------------------------------------	
-- Se�ales internas MIPS	
	CONSTANT ARIT : STD_LOGIC_VECTOR (5 downto 0) := "000001";
	signal load_PC, RegWrite_ID, RegWrite_EX, RegWrite_MEM, RegWrite_WB, RegWrite, Z, Branch_ID, RegDst_ID, RegDst_EX, ALUSrc_ID, ALUSrc_EX: std_logic;
	signal MemtoReg_ID, MemtoReg_EX, MemtoReg_MEM, MemtoReg_WB, MemWrite_ID, MemWrite_EX, MemWrite_MEM, MemRead_ID, MemRead_EX, MemRead_MEM: std_logic;
	signal PC_in, PC_out, four, PC4, Dirsalto_ID, IR_in, IR_ID, PC4_ID, inm_ext_EX, ALU_Src_out : std_logic_vector(31 downto 0);
	signal BusW, BusA, BusB, BusA_EX, BusB_EX, BusB_MEM, inm_ext, inm_ext_x4, ALU_out_EX, ALU_out_MEM, ALU_out_WB, Mem_out, MDR : std_logic_vector(31 downto 0);
	signal RW_EX, RW_MEM, RW_WB, Reg_Rs_ID, Reg_Rs_EX, Reg_Rt_ID, Reg_Rd_EX, Reg_Rt_EX: std_logic_vector(4 downto 0);
	signal ALUctrl_ID, ALUctrl_EX : std_logic_vector(2 downto 0);
	signal ALU_INT_out, Mux_A_out, Mux_B_out: std_logic_vector(31 downto 0);
	signal IR_op_code: std_logic_vector(5 downto 0);
	signal MUX_ctrl_A, MUX_ctrl_B : std_logic_vector(1 downto 0);
	signal salto_tomado: std_logic;
--Se�ales soluci�n
	signal parar_ID, parar_MIPS, RegWrite_EX_mux_out, Kill_IF, reset_ID, load_ID, load_EX, load_Mem, load_WB : std_logic;
	signal Write_output, write_output_UC : std_logic;
-- Soporte Excepciones--
	signal MIPS_status, status_input: std_logic_vector(1 downto 0);
	signal PC_exception_MEM, PC_exception_EX, PC_exception_ID, Exception_LR_output: std_logic_vector(31 downto 0);
	signal Exception_accepted, RTE_ID, RTE_EX, reset_EX, reset_MEM: std_logic;													
	signal Undef: std_logic;
-- Bit validez etapas
	signal valid_I_IF, valid_I_ID,  valid_I_EX, valid_I_EX_in, valid_I_MEM, valid_I_MEM_in, valid_I_WB: std_logic;
-- contadores
	signal cycles: std_logic_vector(15 downto 0);
	signal Ins, data_stalls, control_stalls, Mem_stalls, Exceptions, Exception_cycles: std_logic_vector(7 downto 0);
	signal inc_cycles, inc_I, inc_data_stalls, inc_control_stalls, inc_Mem_stalls, inc_Exceptions, inc_Exception_cycles : std_logic;
--se�ales de control del muxes 4 a 1 que hemos a�adido para la escritura en BR para el jal
	signal ctrl_Mux4a1_escritura_BR: std_logic_vector (1 downto 0);
-- Instrucciones jal y ret
	-- COMPLETAR:
	--nuevas se�ales para transmitir informaci�n para la instruci�n jal y ret
	--si necesit�is propagar las se�ales a otras etapas, definid las se�ales necesarias. Ejemplo: jal_EX, jal_MEM...
	signal PC4_EX, PC4_MEM, PC4_WB : std_logic_vector(31 downto 0);
	signal jal_ID, jal_EX, jal_MEM, jal_WB, ret_ID : std_logic; 	
	

begin

	-- ****************************************************************************************************
	pc: reg generic map (size => 32)
			port map (	Din => PC_in, clk => clk, reset => reset, load => load_PC, Dout => PC_out);
	
	------------------------------------------------------------------------------------
	-- Completar:
	-- load_pc vale '1' porque en la versi�n actual el procesador no para nunca
	-- Si queremos detener una instrucci�n en la etapa fetch habr� que ponerlo a '0'
	-- Si paramos el MIPS o la etapa ID, tambi�n hay que parar en IF. 
	-- Interacci�n con las excepciones:
	-- Si el procesador entero est� parado no procesamos la excepci�n
	-- Si estamos parados en ID, s� que la procesamos (nos da igual la instrucci�n en ID, la vamos a matar)
	load_PC <= ((NOT Parar_ID AND NOT Parar_Mips) OR Exception_accepted);
	-- Fin completar;
	------------------------------------------------------------------------------------
	------------------------------------------------------------------------------------
	 -- la x en x"00000004" indica que est� en hexadecimal
	adder_4: adder32 port map (Din0 => PC_out, Din1 => x"00000004", Dout => PC4);
	------------------------------------------------------------------------------------
	-- COMPLETAR: NUEVO MUX PARA PC
	-- EN lugar de usar un componente mux lo vamos a hacer con un when else. Eso nos permite cambiar el tama�o del mux de forma sencilla sin tener que modificar el componente.		
	-- Completar: sustituir los (--completar) por la condici�n correcta y descomentar el c�digo. Ver ejemplo para el data abort.
	-- Este c�digo es el mux de entrada al PC: elige entre PC+4, la direcci�n de salto generada en ID, la direcci�n de la rutina de tratamiento de la excepci�n, o la direcci�n de retorno de la excepci�n
	-- El orden asigna prioridad si se cumplen dos o m�s condiciones	
		
	PC_in <= 	x"00000008" 		when (Exception_accepted = '1') and (Data_abort = '1') else -- Si llega un data abort saltamos a la direcci�n 0x00000008
				x"0000000C" 		when (Exception_accepted = '1') and (UNDEF = '1') else -- Si llega un UNDEF saltamos a la direcci�n 0x0000000C
				x"00000004" 		when (Exception_accepted = '1') and (IRQ = '1') else -- Si llega una IRQ saltamos a la direcci�n 0x00000004
				Exception_LR_output when RTE_ID = '1' else 	--@ retorno. Si es una RTE volvemos a la @ que ten�amos almacenada en el Exception_LR		
				BusA				when RET_ID = '1' else -- Entrada para el RET, poned lo que toque: 				when RET_ID = '1' else 	--Se elige la direcci�n almacenada en el registro Rs que sale por el puerto A del BR
				Dirsalto_ID 		when salto_tomado = '1' else --@ Salto de las BEQ y JAL. Las RTE y RET tambi�n puede activar la se�al de salto, pero por el orden del when se elegir�n las opciones anteriores
				PC4; -- PC+4
				
								
	------------------------------------------------------------------------------------
	-- Memoria de instrucciones. Tiene el puerto de escritura inhabilitado (WE='0')
	Mem_I: memoriaRAM_I PORT MAP (CLK => CLK, ADDR => PC_out, Din => x"00000000", WE => '0', RE => '1', Dout => IR_in);
	------------------------------------------------------------------------------------
	-- NUEVO
	-- Reset del banco ID: reseteamos el banco si hay una excepci�n aceptada, ya que en ese caso se matan las intrucciones en IF, ID y EX 
	reset_ID <= (reset or Exception_accepted);
	
	--------------------------------------------------------------
	-- NUEVO: en pr�cticas todas las instrucciones son v�lidas. En este procesador a veces debemos invalidar una instrucci�n
	-- anulaci�n de la instrucci�n. Si en ID se detecta que la instrucci�n de IF no debe ejecutarse se desactiva la se�al valid_I_IF
	valid_I_IF <= not(kill_IF);
	-----------------------------------------------------------------
	-- NUEVO: en pr�cticas no se paraba nunca.
	-- Parar_ID detiene la etapa ID y �tambi�n debe parar la anterior! Eso �ltimo lo deb�is hacer vosotros.
	-- Completar: tambi�n hay que parar cuando se active parar_MIPS
	load_ID <= not(parar_ID) and not(parar_MIPS); 

	Banco_IF_ID: Banco_ID port map (	IR_in => IR_in, PC4_in => PC4, clk => clk, reset => reset_ID, load => load_ID, IR_ID => IR_ID, PC4_ID => PC4_ID, 
										--Nuevo
										valid_I_IF => valid_I_IF, valid_I_ID => valid_I_ID,  
										PC_exception => PC_out, PC_exception_ID => PC_exception_ID); 
	--
	------------------------------------------Etapa ID-------------------------------------------------------------------
	Reg_Rs_ID <= IR_ID(25 downto 21);
	Reg_Rt_ID <= IR_ID(20 downto 16);
	--------------------------------------------------
	-- BANCOS DE REGISTROS
	
	-- s�lo se escribe si la instrucci�n en WB es v�lida
	RegWrite <= RegWrite_WB and valid_I_WB;
	
	INT_Register_bank: BReg PORT MAP (clk => clk, reset => reset, RA => Reg_Rs_ID, RB => Reg_Rt_ID, RW => RW_WB, BusW => BusW, RegWrite => RegWrite, BusA => BusA, BusB => BusB);
	
	-------------------------------------------------------------------------------------
	sign_ext: Ext_signo port map (inm => IR_ID(15 downto 0), inm_ext => inm_ext);
	
	two_bits_shift: two_bits_shifter	port map (Din => inm_ext, Dout => inm_ext_x4);
	
	adder_dir: adder32 port map (Din0 => inm_ext_x4, Din1 => PC4_ID, Dout => Dirsalto_ID);
	
	Z <= '1' when (busA=busB) else '0';
	
	-------------------------------------------------------------------------------------
	IR_op_code <= IR_ID(31 downto 26);
	
	-- Si la Instrucci�n en ID no es v�lida se activa UNDEF
	UC_seg: UC port map (valid_I_ID => valid_I_ID, IR_op_code => IR_op_code, Branch => Branch_ID, RegDst => RegDst_ID,  ALUSrc => ALUSrc_ID, MemWrite => MemWrite_ID,  
								MemRead => MemRead_ID, MemtoReg => MemtoReg_ID, RegWrite => RegWrite_ID, 
								-- se�ales para las instrucciones de la pr�ctica:
								jal => jal_ID, ret => ret_ID,
								--Se�ales nuevas
								-- RTE
								RTE => RTE_ID,
								--Fin se�ales nuevas
								undef => undef);
	
	
	------------------------------------------------------------------------------------							
	-- Salto tomado se debe activar cada vez que la instrucci�n en D produzca un salto en la ejecuci�n.
	-- Eso incluye los saltos tomados en los BEQs (Z AND Branch_ID) y los saltos debidos a una RTE
	-- IMPORTANTE: si la instrucci�n no es v�lida no se salta, pero eso ya lo comprueb la UC
	salto_tomado <= ((Z AND Branch_ID) or RTE_ID or jal_ID or ret_ID);
								
	------------------------Unidad de detenci�n-----------------------------------
	-- NUEVO PROYECTO
	-- Deb�is completar la unidad para que genere las siguientes se�ales correctamente:
	-- Kill_IF: mata la instrucci�n que se est� leyendo en la etapa IF (para que no se ejecute)
	-- parar_ID: detiene la ejecuci�n de las etpas ID e IF cuando hay riesgos
	-- IMPORTANTE: para detectar un riesgo, primero comprobar que las instrucciones implicadas son v�lidas. �Las instrucciones invalidas no generan riesgos porque no son instrucciones que se vayan a ejecutar
	-- IMPORTANTE: para detectar una relacci�n productor consumidor deb�is comprobar las dos cosas, que hay un productor, y tambi�n un consumidor. 
	-- No todas las instrucciones producen/escriben datos en el BR, ni todas consumen/leen los dos operandos (rs y rt)
	-- NUEVO: parar_MIPS: se utiliza para parar todo el procesador cuando la memoria no puede realizar la operaci�n solicitada en el ciclo actual (es decir cuando Mem_ready es 0). 
		
	
	-------------------------------------------------------------------------------------
	
	Unidad_detencion_riesgos: UD port map (	valid_I_ID => valid_I_ID, valid_I_EX => valid_I_EX, valid_I_MEM => valid_I_MEM, Reg_Rs_ID => Reg_Rs_ID, Reg_Rt_ID => Reg_Rt_ID, MemRead_EX => MemRead_EX, RW_EX => RW_EX, RegWrite_EX => RegWrite_EX,
											RW_Mem => RW_Mem, RegWrite_Mem => RegWrite_Mem, IR_op_code => IR_op_code, salto_tomado => salto_tomado,
											kill_IF => kill_IF, parar_ID => parar_ID,
											JAL_EX => JAL_EX, JAL_MEM => JAL_MEM,
											Mem_ready => Mem_ready, parar_MIPS => parar_MIPS);
								
	-- NUEVO
	-- Si nos paran en ID marcamos como invalida la instrucci�n que mandamos a la etapa EX
	-- La instrucci�n de EX ser� v�lida el pr�ximo ciclo, si lo es la de ID y no hay detenci�n
	valid_I_EX_in	<=  valid_I_ID and not( parar_ID);				
				
	-------------------------------------------------------------------------------------
	-- si la operaci�n es aritm�tica (es decir: IR_op_code= "000001") miro el campo funct
	-- como s�lo hay 4 operaciones en la alu, basta con los bits menos significativos del campo func de la instrucci�n	
	-- si no es aritm�tica le damos el valor de la suma (000)
	ALUctrl_ID <= IR_ID(2 downto 0) when IR_op_code= ARIT else "000"; 
	
	-- NUEVO
	-- Reset del banco EX: reseteamos el banco si hay una excepci�n aceptada, ya que en ese caso se matan las intrucciones en IF, ID y EX
	reset_EX <= (reset or Exception_accepted);
	-- Banco ID/EX parte de enteros
	-- COMPLETAR: Si para_MIPS se activa hay que detener la ejecuci�n, y mantener cada instrucci�n en su etapa actual
	load_EX <= '1' when (parar_MIPS = '0') else '0';
	Banco_ID_EX: Banco_EX PORT MAP ( 	clk => clk, reset => reset_EX, load => load_EX, busA => busA, busB => busB, busA_EX => busA_EX, busB_EX => busB_EX,
						RegDst_ID => RegDst_ID, ALUSrc_ID => ALUSrc_ID, MemWrite_ID => MemWrite_ID, MemRead_ID => MemRead_ID,
						MemtoReg_ID => MemtoReg_ID, RegWrite_ID => RegWrite_ID, RegDst_EX => RegDst_EX, ALUSrc_EX => ALUSrc_EX,
						MemWrite_EX => MemWrite_EX, MemRead_EX => MemRead_EX, MemtoReg_EX => MemtoReg_EX, RegWrite_EX => RegWrite_EX,
						-- Nuevo: para la anticipaci�n
						Reg_Rs_ID => Reg_Rs_ID,
						Reg_Rs_EX => Reg_Rs_EX,
						-- Nuevo: para llevar el PC a la etapa MEM
						PC_exception_ID => PC_exception_ID, PC_exception_EX => PC_exception_EX, 
						-- Nuevo para el retorno de la excepci�n
           				RTE_ID =>  RTE_ID, RTE_EX => RTE_EX,
						--Fin nuevo
						ALUctrl_ID => ALUctrl_ID, ALUctrl_EX => ALUctrl_EX, inm_ext => inm_ext, inm_ext_EX=> inm_ext_EX,
						Reg_Rt_ID => IR_ID(20 downto 16), Reg_Rd_ID => IR_ID(15 downto 11), Reg_Rt_EX => Reg_Rt_EX, Reg_Rd_EX => Reg_Rd_EX, 
						valid_I_EX_in => valid_I_EX_in, valid_I_EX => valid_I_EX,
						-- Puertos de extensi�n. Inicialmente est�n desconectados
						ext_word_1_ID => PC4_ID, ext_word_2_ID => x"00000000", ext_signal_1_ID => jal_ID, ext_signal_2_ID => '0',
						ext_word_1_EX => PC4_EX, ext_word_2_EX => open, ext_signal_1_EX => jal_EX, ext_signal_2_EX => open
						);  		
	
	------------------------------------------Etapa EX-------------------------------------------------------------------
	---------------------------------------------------------------------------------
	-- NUEVO
	-- Unidad de anticipaci�n de enteros incompleta. Deb�is dise�adla teniendo en cuenta que instrucciones lee y escribe cada instrucci�n
	-- Entradas: Reg_Rs_EX, Reg_Rt_EX, RegWrite_MEM, RW_MEM, RegWrite_WB, RW_WB
	-- Salidas: MUX_ctrl_A, MUX_ctrl_B
	Unidad_Ant_INT: UA port map (	valid_I_MEM => valid_I_MEM, valid_I_WB => valid_I_WB, Reg_Rs_EX => Reg_Rs_EX, Reg_Rt_EX => Reg_Rt_EX, RegWrite_MEM => RegWrite_MEM,
									RW_MEM => RW_MEM, RegWrite_WB => RegWrite_WB, RW_WB => RW_WB, MUX_ctrl_A => MUX_ctrl_A, MUX_ctrl_B => MUX_ctrl_B);
	-- Muxes para la anticipaci�n
	Mux_A: mux4_1 port map  ( DIn0 => BusA_EX, DIn1 => ALU_out_MEM, DIn2 => busW, DIn3 => x"00000000", ctrl => MUX_ctrl_A, Dout => Mux_A_out);
	Mux_B: mux4_1 port map  ( DIn0 => BusB_EX, DIn1 => ALU_out_MEM, DIn2 => busW, DIn3 => x"00000000", ctrl => MUX_ctrl_B, Dout => Mux_B_out);
	
	----------------------------------------------------------------------------------
	
	
	muxALU_src: mux2_1 port map (Din0 => Mux_B_out, DIn1 => inm_ext_EX, ctrl => ALUSrc_EX, Dout => ALU_Src_out);
	
	ALU_MIPs: ALU PORT MAP ( DA => Mux_A_out, DB => ALU_Src_out, ALUctrl => ALUctrl_EX, Dout => ALU_out_EX);
	
	
	mux_dst: mux2_5bits port map (Din0 => Reg_Rt_EX, DIn1 => Reg_Rd_EX, ctrl => RegDst_EX, Dout => RW_EX);
	
	reset_MEM <= (reset);
	-- Nuevo: Si llega una excepci�n matamos la instrucci�n de EX, por tanto no dbeemos dejar que pase a MEM. 
	-- Como en otros casos podmeos usar el bit de validez
	valid_I_MEM_in <= valid_I_EX and not(Exception_accepted);
	-- Nuevo: si paramos en EX no hay que cargar una instrucci�n nueva en la etap MEM
	-- COMPLETAR: Si para_MIPS se activa hay que detener la ejecuci�n, y mantener cada instrucci�n en su etapa actual
	load_MEM <= '1' when (parar_MIPS = '0') else '0';
	Banco_EX_MEM: Banco_MEM PORT MAP ( 	ALU_out_EX => ALU_out_EX, ALU_out_MEM => ALU_out_MEM, clk => clk, reset => reset_MEM, load => load_MEM, MemWrite_EX => MemWrite_EX,
										MemRead_EX => MemRead_EX, MemtoReg_EX => MemtoReg_EX, RegWrite_EX => RegWrite_EX, MemWrite_MEM => MemWrite_MEM, MemRead_MEM => MemRead_MEM,
										MemtoReg_MEM => MemtoReg_MEM, RegWrite_MEM => RegWrite_MEM, 
										--COMPLETAR: Si usamos BusB_EX los sw no podr�n hacer cortos en rt
										-- �qu� se�al debemos usar para utilizar la red de cortos?
										-- Soluci�n
										BusB_EX => Mux_B_out,  
										--FIN COMPLETAR
										BusB_MEM => BusB_MEM, RW_EX => RW_EX, RW_MEM => RW_MEM,
										valid_I_EX => valid_I_MEM_in, valid_I_MEM => valid_I_MEM,
										--Nuevo: para llevar el PC a la etapa MEM	
										PC_exception_EX => PC_exception_EX, PC_exception_MEM => PC_exception_MEM, 
										-- Puertos de extensi�n. Inicialmente est�n desconectados
										ext_word_1_EX => PC4_EX, ext_word_2_EX => x"00000000", ext_signal_1_EX => jal_EX, ext_signal_2_EX => '0',
										ext_word_1_MEM => PC4_MEM, ext_word_2_MEM => open, ext_signal_1_MEM => jal_MEM, ext_signal_2_MEM => open
										);
	
	--
	------------------------------------------Etapa MEM-------------------------------------------------------------------
	--
	-- COMPLETAR: M�dulo para gestionar las excepciones 
	Exception_Mng:  Exception_manager PORT MAP (clk => clk, reset => reset, IRQ => IRQ, Data_abort => Data_abort, undef => undef, RTE_EX => RTE_EX,
												RTE_ID => RTE_ID, valid_I_ID => valid_I_ID, valid_I_EX => valid_I_EX, valid_I_MEM => valid_I_MEM,
												parar_MIPS => parar_MIPS, PC_out => PC_out, PC_exception_EX => PC_exception_EX, PC_exception_ID => PC_exception_ID,
												Exception_accepted => Exception_accepted, Exception_LR_output => Exception_LR_output);  
           	

	
	-- NUEVO: en esta etapa accedemos al subsistema de IO/MD. Como lo hemos sacado del MIPS no aparece el componente de forma directa, solo las se�ales. 
	-- El componente est� en el SOC
	-- Interface with the IO/MD subsystem
	WE <= MemWrite_MEM and valid_I_MEM; --Se�al de escritura para el subsistema de IO/MD. S�lo se escribe si es una instrucci�n v�lida
	RE <= MemRead_MEM and valid_I_MEM; --Se�al de escritura para el subsistema de IO/MD. S�lo se lee si es una instrucci�n v�lida
	ADDR <= ALU_out_MEM; --@ que envia el MIPS al subsistema de IO/MD
	Dout <= BusB_MEM; --Dato que envia el MIPS al subsistema de IO/MD
	-- En el interfaz tambi�n est� la entrada Mem_ready que nos avisa de si el subsistema de IO/MD va a realizar en este ciclo la orden del MIPS
	Mem_out <= Din; --Dato que se recibe del subsistema de IO/MD	     	
	    	
	-- Nuevo: si paramos en EX no hay que cargar una instrucci�n nueva en la etap MEM
	-- COMPLETAR: Si para_MIPS se activa hay que detener la ejecuci�n, y mantener cada instrucci�n en su etapa actual
	load_WB <= '1' when (parar_MIPS = '0') else '0';
	Banco_MEM_WB: Banco_WB PORT MAP ( 	ALU_out_MEM => ALU_out_MEM, ALU_out_WB => ALU_out_WB, Mem_out => Mem_out, MDR => MDR, clk => clk, reset => reset, load => load_WB, 
										MemtoReg_MEM => MemtoReg_MEM, RegWrite_MEM => RegWrite_MEM, MemtoReg_WB => MemtoReg_WB, RegWrite_WB => RegWrite_WB, 
										RW_MEM => RW_MEM, RW_WB => RW_WB,
										valid_I_WB_in => valid_I_MEM, valid_I_WB => valid_I_WB,
										-- Puertos de extensi�n. Inicialmente est�n desconectados
										ext_word_1_MEM => PC4_MEM, ext_word_2_MEM => x"00000000", ext_signal_1_MEM => jal_MEM, ext_signal_2_MEM => '0',
										ext_word_1_WB => PC4_WB, ext_word_2_WB => open, ext_signal_1_WB => jal_WB, ext_signal_2_WB => open
										);
	
	--
	------------------------------------------Etapa WB-------------------------------------------------------------------
	--	Mux 4 a 1. Inicialmente s�lo se usan dos entradas, y las otras dos est�n desconectadas, pero se pueden usar para las nuevas instrucciones	
	--  Para ello hay que realizar las conexiones necesarias, y ajustar la se�al de control del multiplexor			
	-- Completar con vuestra soluci�n de la pr�ctica 3
	ctrl_Mux4a1_escritura_BR(1) <= jal_WB;
	ctrl_Mux4a1_escritura_BR(0) <= MemtoReg_WB;		
	mux_busW: mux4_1 port map (Din0 => ALU_out_WB, DIn1 => MDR, DIn2 => PC4_WB, DIn3 => x"00000000", ctrl => ctrl_Mux4a1_escritura_BR, Dout => busW);
	

--------------------------------------------------------------------------------------------------
----------- COMPLETAR
----------- Contadores de eventos.  Nos permiten calcular m�tricas de rendimiento como el CPI y comprobar que se han producido las detenciones esperadas
-------------------------------------------------------------------------------------------------- 
	-- Contador de ciclos totales
	cont_cycles: counter 	generic map (size => 16)
							port map (clk => clk, reset => reset, count_enable => inc_cycles, count => cycles);
	-- Contador de Instrucciones ejecutadas
	cont_I: counter 		generic map (size => 8)
							port map (clk => clk, reset => reset, count_enable => inc_I, count => Ins);
	-- Contador de n�mero de excepciones							
	cont_Exceptions: counter 		generic map (size => 8)
							port map (clk => clk, reset => reset, count_enable => inc_Exceptions, count => Exceptions);
	-- Contador de ciclos ejecutando excepciones						
	cont_Exceptions_cycles : counter generic map (size => 8)
							port map (clk => clk, reset => reset, count_enable => inc_Exception_cycles, count => Exception_cycles);
	-- Contadores de detenciones
	-- Nos permiten analizar las penalizaciones en el rendimiento
	-- IMPORTANTE: cuando se pare se debe incrementar s�lo uno de los contadores. Por ejemplo, si hay un riesgo de datos, y la memoria para el procesador 4 ciclos, esas 4 paradas se deben a la memoria, no al riesgo de datos. Cuando la memoria permita continuar, es cuando se producir� la detenci�n debido al riesgo, y s�lo entyonces deber� contarse.
	-- Contador de detenciones por riesgos de datos						
	cont_data_stalls: counter generic map (size => 8)
							port map (clk => clk, reset => reset, count_enable => inc_data_stalls, count => data_stalls);
	-- Contador de detenciones por riesgos de control							
	cont_control_stalls: counter generic map (size => 8)
							port map (clk => clk, reset => reset, count_enable => inc_control_stalls, count => control_stalls);
	-- Contador de paradas en memoria
	cont_Memory_stalls : counter generic map (size => 8)
							port map (clk => clk, reset => reset, count_enable => inc_Mem_stalls, count => Mem_stalls);						
							
							
							
	------------------------------------------------------------------------------------
	-- Completar:
	inc_cycles <= '1';--Done
	inc_I <= '1' when ((valid_I_WB = '1') and (PARAR_MIPS= '0')) else '0';
	inc_data_stalls <= '1' when ((Parar_ID = '1') AND (inc_Mem_stalls = '0')) else '0';
	inc_control_stalls <= '1' when ((Kill_IF = '1') AND (inc_Mem_stalls = '0') AND (inc_data_stalls = '0')) else '0';
	inc_Exceptions <= '1' when (Exception_accepted = '1') else '0';
	inc_Mem_stalls <= '1' when (parar_MIPS = '1') else '0';
	------------------------------------------------------------------------------------			
end Behavioral;

