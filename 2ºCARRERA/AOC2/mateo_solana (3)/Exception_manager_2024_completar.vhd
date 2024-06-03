----------------------------------------------------------------------------------
-- Authors: Mateo Lorente, Diego  --- 873338
--			Solana Melero, Carlos --- 872815
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity Exception_manager is
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
end Exception_manager;

architecture Behavioral of Exception_manager is

component reg is
    generic (size: natural := 32);  -- por defecto son de 32 bits, pero se puede usar cualquier tama�o
	Port ( Din : in  STD_LOGIC_VECTOR (size -1 downto 0);
           clk : in  STD_LOGIC;
	   reset : in  STD_LOGIC;
           load : in  STD_LOGIC;
           Dout : out  STD_LOGIC_VECTOR (size -1 downto 0));
end component;

-- Soporte Excepciones--
	signal MIPS_status, status_input: std_logic_vector(1 downto 0);
	signal Return_I : std_logic_vector(31 downto 0);
	signal update_status, Exception_accepted_internal: std_logic;		
	-- ****************************************************************************************************
	-- Gesti�n de Excepciones: 
	--		* IRQ: es una entrada del MIPs
	--		* Data_abort: la genera el controlador de memoria cuando recibe una direcci�n no alienada, o fuera del rango de la memoria
	--		* UNDEF: la genera la unidad de control cuando le llega una instrucci�n v�lida con un c�digo de operaci�n desconocido
	-- ****************************************************************************************************
	-------------------------------------------------------------------------------------------------------------------------------
	-- Status_register	 
	-- el registro tiene como entradas y salidas vectores de se�ales cuya longitud se indica en size. En este caso es un vector de tama�o 2
	-- El bit m�s significativo permite deshabilitar (valor 1) o habilitar las excepciones (valor 0)
	-- El bit menos significativo informa si estamos en modo Excepci�n o estamos en modo normal
Begin	
	status_reg: reg generic map (size => 2)
			port map (	Din => status_input, clk => clk, reset => reset, load => update_status, Dout => MIPS_status);
	------------------------------------------------------------------------------------
	-- Completar: falta la l�gica que detecta cu�ndo se va a procesar una excepci�n: cuando se recibe una de las se�ales (IRQ, Data_abort y Undef) y las excepciones est�n habilitadas (MIPS_status(1)='0')
	-- IMPORTANTE: No hay que actualizar nada si el MIPS est� parado
	
	update_status	<= '1' when (((parar_MIPS = '0') AND (RTE_ID = '1')) OR (Exception_accepted_internal = '1')) else '0';
	
	-- Completar:se procesa una excepci�n si se recibe IRQ, Abort o Undef y las excepciones est�n habilitadas (MIPS_status(1)='0') y el procesador no est� parado (parar_MIPS = '0')
	-- se define una se�al interna para poder leerla en el m�dulo (las se�ales de salida, como Exception_accepted, no se pueden leer)
	Exception_accepted_internal <= '1' when ((parar_MIPS = '0') AND (MIPS_status(1)='0') AND ((IRQ ='1') OR (Data_abort = '1') OR (undef = '1'))) else '0';
	Exception_accepted <= Exception_accepted_internal;
	-- Fin completar;
	------------------------------------------------------------------------------------
				
	-- multiplexor para elegir la entrada del registro de estado
	-- En este procesador s�lo hay dos opciones ya que al entrar en modo excepci�n se deshabilitan las excepciones:
	-- 		* "11" al entrar en una IRQ (Excepciones deshabilitadas y modo Excepci�n)
	--		* "00" en el resto de casos
	-- Podr�a hacerse con un bit, pero usamos dos para permitir ampliaciones)
	status_input	<= 	"11" when (Exception_accepted_internal = '1') else "00";							
	
	------------------------------------------------------------------------------------
	-- Al procesar una excepci�n las instrucciones que est�n en Mem y WB contin�an su ejecuci�n. El resto se matan
	-- Para retornar se debe eligir la siguiente instrucci�n v�lida. Para ello tenemos sus direcciones almacenadas en:
	-- PC_exception_EX y PC_exception_ID, y sus bits de validez en valid_I_EX y valid_I_ID
	-- Si no hay v�lidas se elige el valor del PC.
	-- IMPORTANTE: Si la instrucci�n en la etapa EX es una RTE no debe elegirse, ya que es una instrucci�n que ya se ha ejecutado por completo (el retorno se hace en ID), y que ha
	-- ha perdido la informaci�n que necesita. Es decir, su LR, porque si ha saltado otra excepci�n lo habr� borrado.
	-- Para evitar corromper la ejecuci�n a�adimos la comprobaci�n RTE_EX='0'
	Return_I	<= 	PC_exception_EX when ((valid_I_EX = '1')AND(RTE_EX = '0')) else 	
					PC_exception_ID when (valid_I_ID = '1') else
					PC_out;		
	------------------------------------------------------------------------------------	
	-- Exception_LR: almacena la direcci�n a la que hay que retornar tras una excepci�n	 
	-- Vamos a guardar la direcci�n seleccionada en el MUX de arriba
	Exception_LR: reg generic map (size => 32)
			port map (	Din => Return_I, clk => clk, reset => reset, load => Exception_accepted_internal, Dout => Exception_LR_output);
			
end Behavioral;
