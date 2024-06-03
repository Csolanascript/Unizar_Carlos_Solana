----------------------------------------------------------------------------------
-- Authors: Mateo Lorente, Diego  --- 873338
--			Solana Melero, Carlos --- 872815
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use ieee.std_logic_unsigned.all;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;
-- Memoria RAM de 128 oalabras de 32 bits
entity IO_Data_Memory_Subsystem is port (
	  CLK : in std_logic;
	  reset: in std_logic; 		
	  -- READ/Write ports with the MIPS core
	  ADDR : in std_logic_vector (31 downto 0); --Dir solicitada por el Mips
          Din : in std_logic_vector (31 downto 0);--entrada de datos desde el Mips
	  WE : in std_logic;		-- write enable	del MIPS
	  RE : in std_logic;		-- read enable del MIPS			  
	  Mem_ready: out std_logic; -- indica si puede hacer la operaci�n solicitada en el ciclo actual
	  Data_abort: out std_logic; --indica que el �ltimo acceso a memoria ha sido un error
	  Dout : out std_logic_vector (31 downto 0); --dato que se env�a al Mips
	  -- SoC Input/Output ports
	  Ext_IRQ	: 	in  STD_LOGIC;   --External interrupt signal
	  INT_ACK	: 	out  STD_LOGIC;  --Signal to acknowledge the interrupt
	  MIPS_IRQ	: 	out  STD_LOGIC;  -- IRQ signal for the MIPS core. It may combine several IRQ signals from different sources 
	  IO_input: in std_logic_vector (31 downto 0); --dato que viene de una entrada del sistema
	  IO_output : out  STD_LOGIC_VECTOR (31 downto 0) -- 32 bits de salida para el MIPS para IO
		  ); 
end IO_Data_Memory_Subsystem;

-- En este proyecto el IO_Data_Memory_Subsystem est� compuesto por una MD de 128 palabras con acceso alineado, y dos registros mapeados en memoria, uno de entrada y otro de salida
-- En el P2 esta parte ser� m�s complicada
-- Para poder comprobar que el MIPS para cuando el subsistema no est� preparado, Mem_ready se fuerza a '0' 3 de cada 4 ciclos. 
-- Por tanto un lw o sw podr�n generar 0, 1, 2 o 3 ciclos de parada.
architecture Behavioral of IO_Data_Memory_Subsystem is

component reg is
    generic (size: natural := 32);  -- por defecto son de 32 bits, pero se puede usar cualquier tama�o
	Port ( Din : in  STD_LOGIC_VECTOR (size -1 downto 0);
           clk : in  STD_LOGIC;
	   reset : in  STD_LOGIC;
           load : in  STD_LOGIC;
           Dout : out  STD_LOGIC_VECTOR (size -1 downto 0));
end component;	

component counter is
 	generic (   size : integer := 10);
	Port ( 	clk : in  STD_LOGIC;
       		reset : in  STD_LOGIC;
       		count_enable : in  STD_LOGIC;
       		count : out  STD_LOGIC_VECTOR (size-1 downto 0));
end component;

type RamType is array(0 to 127) of std_logic_vector(31 downto 0);
-- Contenido Memoria Datos para el c�digo test_IRQ_2024: [256, 1, 8, Nint (0 al empezar), 12, 0x00000AB0, 0, 0x0BAD0C0D, 0, 0, 0�]  
									
--signal RAM : RamType := (  			X"00000100", X"00000001", X"00000008", X"00000000", X"0000000C", X"00000AB0", X"00000000", X"0BAD0C0D", --word 0,1,...
--									X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", --word 8,9,...
--									X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", --word 16,...
--									X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", --word 24,...
--									X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", --word 32,...
--									X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", --word 40,...
--									X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", --word 48,...
--									X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", --word 56,...
--									X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000",  --word 64,...
--									X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", --word 72,...
--									X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", --word 80,...
--									X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", --word 88,...
--									X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", --word 96,...
--									X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", --word 104,...
--									X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", --word 112,...
--									X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000");--word 120,...


-- RAM de datos para el banco de pruebas 9									
signal RAM : RamType := (  		X"00080000", X"00070000", X"00020000", X"00000800", X"00000010", X"00000005", X"FFFFFFFF", X"FFFFFFFF", -- palabras 0,1,2,3,4,5,6,7
									X"000000BB", X"11220044", X"FFFFFFFF", X"FFFFFFFF", X"AA995566", X"20000000", X"30008001", X"00000007", --palabras 8,9,...
									X"20000000", X"20000000", X"30018001", X"02AD6093", X"30008001", X"00000001", X"20000000", X"30002001",
									X"00010900", X"20000000", X"30004000", X"5000102D", X"01000300", X"80000400", X"10000000", X"00000000",
									X"00002000", X"00000000", X"00000000", X"00000002", X"00000000", X"00000000", X"00000000", X"00000002",
									X"00000000", X"00000002", X"00001200", X"00000000", X"00000000", X"00000002", X"00000000", X"00000002",
									X"000006AD", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000",
									X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00100000", X"00000000", X"00000000",
									X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"02000000", X"08010000", X"00000000",
									X"00000000", X"00000000", X"00000010", X"00000000", X"00000000", X"00000000", X"00000002", X"00200000",
									X"00000000", X"20080001", X"00000000", X"00000000", X"00000800", X"40000000", X"00000000", X"00000000",
									X"00000000", X"000009C2", X"00000000", X"00000000", X"00000800", X"00000000", X"00080800", X"00000000",
									X"00080000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000",
									X"00002000", X"00400000", X"00080000", X"00000000", X"00080000", X"00000000", X"50000000", X"00000001",
									X"40020000", X"00000000", X"00000000", X"40000000", X"00000000", X"00000004", X"00000800", X"00800004",
									X"20800000", X"00000004", X"00000000", X"00000004", X"00000000", X"00000000", X"20000000", X"00000004");

signal dir_7:  std_logic_vector(6 downto 0); 
signal unaligned, out_of_range, load_output, Mem_addr, IO_addr, addr_input, addr_output, addr_ack, ack_input, ready, Data_abort_internal:  std_logic;
signal Input_data, Output_data, RAM_Dout: std_logic_vector (31 downto 0);
signal cuenta_retardos : std_logic_vector (1 downto 0);
signal ack_input_vector, INT_ACK_vector : std_logic_vector (0 downto 0);
begin
 
--------------------------------------------------------------------------------------------------
-- Decodificador para detectar si la se�al pertenece a memoria o a los registros mapeados en memoria
-- Direcciones de MEM v�lidas: desde x"00000000" al x"000001FC"
	Mem_addr <= '1' when (ADDR(31 downto 9)= "00000000000000000000000") else '0'; 
-- Direcciones de IO v�lidas:  x"00007000" y x"0000700C" Hay 4 registros direccionables, aunque s�lo se usan 3
	IO_addr <= '1' when (ADDR(31 downto 4) = x"0000700") else '0'; 
	addr_input <= '1' when ((IO_addr='1') and (ADDR(3 downto 0)= "0000")) else '0'; --x"00007000"
	addr_output <= '1' when ((IO_addr='1') and (ADDR(3 downto 0)= "0100")) else '0';--x"00007004"
	addr_ack <= '1' when ((IO_addr='1') and (ADDR(3 downto 0)= "1000")) else '0';--x"00007008"

--------------------------------------------------------------------------------------------
-- MD:    
	-- como la memoria es de 128 plalabras no usamos la direcci�n completa sino s�lo 7 bits. Como se direccionan los bytes, pero damos palabras no usamos los 2 bits menos significativos
	dir_7 <= ADDR(8 downto 2); 
	process (CLK)
	   begin
	       if (CLK'event and CLK = '1') then
	           if (WE = '1') and (Mem_addr = '1') then -- s�lo se escribe si WE vale 1 y la @ pertenece a la memoria
	               RAM(conv_integer(dir_7)) <= Din;
	           end if;
	       end if;
	   end process;
	
	   RAM_Dout <= RAM(conv_integer(dir_7)) when (RE='1') and (Mem_addr = '1') else "00000000000000000000000000000000"; --s�lo se lee si RE vale 1 y la @ pertenece a la memoria
	    
--------------------------------------------------------------------------------------------------
-- Registros Entrada/salida
-- 
-- Input_Reg: Permite leer los 32 bits de entrada del sistema. Su direcci�n asociada es "01000000"
-- Output_Reg: Permite escribir en los 32 bits de entrada del sistema. Su direcci�n asociada es "01000004"
--------------------------------------------------------------------------------------------------
	Input_Reg: reg generic map (size => 32)
					port map (	Din => IO_input, clk => clk, reset => reset, load => '1', Dout => Input_data);
	-- El contenido del Output_Reg es visible desde el exterior			
	load_output <= '1' when (addr_output='1') and (WE = '1') else '0';
	-- 
	Output_Reg: reg generic map (size => 32)
					port map (	Din => Din, clk => clk, reset => reset, load => load_output, Dout => Output_data);
	IO_output <= Output_data	;
	-- ACK_Reg para informar que se ha atendido a la INT externa
	-- S�lo tiene un bit, y se pone a 0 autom�ticamente despu�s de cada escritura. Lo hace para que la RTI sea m�s ligera (as� no hay que poner un '1', y luego un '0' para evitar que la siguiente INT crea que ya la han atendido)
	-- y tambi�n para evitar dar el ACK a dos interrupciones que vayan muy seguidas, cuando s�lo se ha tratado la primera
	-- Si se indica la @ del Ack y se da la orden de escribir se cargar� en el registro el bit 0 de los datos de entrada. En caso contrario se pondr� un 0
	ACK_input <= Din(0) and addr_ack and WE; 
	ACK_input_vector <= (0 => ACK_input);-- El componente registro usa vectores para la entrada y salida (en este caso de 1 bit). Esta l�nea asigna una se�al al bit 0 del vector. Es una forma de pasar de std_logic a STD_logic_vector
	ACK_Reg: reg generic map (size => 1)
					port map (	Din => ACK_input_vector, clk => clk, reset => reset, load => '1', Dout => INT_ACK_vector);
	INT_ACK <= INT_ACK_vector(0);
------------------------
-- DATA ABORT
--------------------------------------------------------------------------------------------------------------------
    -- out_of_range se activa si la direcci�n est� fuera del rango. 
    -- Se activa si la direcci�n no pertenece a MD ni a los registros IO
    out_of_range <= '0' when ((Mem_addr = '1') OR (IO_addr = '1')) else '1';
    -- S�lo vamos a permitir direcciones alineadas. Como leemos palabras de 4 bytes estas deben estar en direcciones m�ltiplos de 4. Es decir, acaban en "00"
    unaligned <= '0' when (ADDR(1 downto 0)= "00") else '1';
    -- Hay un data abort cuando se accede a una direcci�n que no existe, o se realiza un acceso no alineado.
    -- �Pero s�lo si se est� haciendo un acceso a memoria! Si WE y RE valen cero no se est� accediendo a memoria, por tanto da igual el valor de la direcci�n
    Data_abort_internal <= (out_of_range or unaligned) and (WE or RE);
    Data_abort <= Data_abort_internal; 

--------------------------------------------------------------------------------------------------
----------- Salidas para el Mips
-------------------------------------------------------------------------------------------------- 
Dout <= x"000BAD00" when ready = '0' else -- Valor para depuraci�n, si aparece en vuestros registros est�is leyendo datos cuando la memoria no est� preparada
		x"BAD0ADD0" when Data_abort_internal = '1' else -- Valor para depuraci�n, si aparece en vuestros registros est�is leyendo datos cuando la @ no es correcta
		RAM_Dout 	when Mem_addr = '1' else -- la memoria comprueba internamente que RE valga '1'
		Input_data 	when ((addr_input = '1') and (RE='1')) else -- se usa para mandar el dato del registro de entrada
		Output_data when ((addr_output = '1') and (RE='1')) else -- se usa para mandar el dato del registro de salida
		x"00000000"; -- Cuando no se pide nada

--Interrupts: There may be many interrupt sources. But currently we only have the external one

MIPS_IRQ <= Ext_IRQ;
		
--------------------------------------------------------------------------------------------------------------------
-- MEMORY READY
--------------------------------------------------------------------------------------------------------------------
-- Para poder comprobar que el MIPS para cuando el subsistema no est� preparado, Mem_ready se fuerza a '0' 3 de cada 4 ciclos. 
-- Sencillamente un contador cuenta de 0 a 3, y activa mem_ready s�lo al llegar a 3.
    cont_retardos: counter 	generic map (size => 2)	port map (clk => clk, reset => reset, count_enable => '1', count => cuenta_retardos);
    -- se�al interna de mem_ready para poder leerla
    ready <= '1' when (cuenta_retardos = "11")or((WE='0')and(RE='0')) else '0';
    Mem_ready <= ready;
    

end Behavioral;

