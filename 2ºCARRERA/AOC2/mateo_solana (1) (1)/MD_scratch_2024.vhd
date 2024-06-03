----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    14:12:11 04/04/2014 
-- Design Name: 
-- Module Name:    DMA - Behavioral 
-- Project Name: 
-- Target Devices: 
-- Tool versions: 
-- Description: 
--
-- Dependencies: 
--
-- Revision: 
-- Revision 0.01 - File Created
-- Additional Comments: 
--
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;


entity MD_scratch is port (
		  CLK : in std_logic;
		  reset: in std_logic;
		  Bus_Frame: in std_logic; -- indica que el master quiere más datos
		  bus_Read: in std_logic;
		  bus_Write: in std_logic;
		  Bus_Addr : in std_logic_vector (31 downto 0); --Direcciones 
		  Bus_Data : in std_logic_vector (31 downto 0); --Datos  
		  MD_Bus_DEVsel: out std_logic; -- para avisar de que se ha reconocido que la dirección pertenece a este módulo
		  MD_Bus_TRDY: out std_logic; -- para avisar de que se va a realizar la operación solicitada en el ciclo actual
		  MD_send_data: out std_logic; -- para enviar los datos al bus
        MD_Dout : out std_logic_vector (31 downto 0)		  -- salida de datos
		  );
end MD_scratch;

architecture Behavioral of MD_scratch is


-- misma memoria que en el proyecto anterior
component RAM_64_32 is port (
		  CLK : in std_logic;
		  enable: in std_logic; --solo se lee o escribe si enable está activado
		  ADDR : in std_logic_vector (31 downto 0); --Dir 
        Din : in std_logic_vector (31 downto 0);--entrada de datos para el puerto de escritura
        WE : in std_logic;		-- write enable	
		  RE : in std_logic;		-- read enable		  
		  Dout : out std_logic_vector (31 downto 0));
end component;

component reg32 is
    Port ( Din : in  STD_LOGIC_VECTOR (31 downto 0);
           clk : in  STD_LOGIC;
			  reset : in  STD_LOGIC;
           load : in  STD_LOGIC;
           Dout : out  STD_LOGIC_VECTOR (31 downto 0));
end component;


component reg is
    generic (size: natural := 32);  -- por defecto son de 32 bits, pero se puede usar cualquier tamaño
	Port ( Din : in  STD_LOGIC_VECTOR (size -1 downto 0);
           clk : in  STD_LOGIC;
		   reset : in  STD_LOGIC;
           load : in  STD_LOGIC;
           Dout : out  STD_LOGIC_VECTOR (size -1 downto 0));
end component;

signal MD_Scratch_enable,load_reg, load_addr, addr_in_range, BUS_RE, BUS_WE, Scratch_RE, Scratch_WE: std_logic;
signal addr_reg:  STD_LOGIC_VECTOR (6 downto 0);
signal MD_Scratch_Dout_int, MD_scratch_addr: STD_LOGIC_VECTOR (31 downto 0);
type state_type is (Inicio, Acceso, Envio); 
signal state, next_state : state_type; 
signal load_control, Internal_read, Internal_write: std_logic; --signals to store inputs bus_read, and bus_write
begin
---------------------------------------------------------------------------
-- Decodificador: identifica cuando la dirección pertenece a la MD: (X"10000000"-X"100000FF")
---------------------------------------------------------------------------
-- Se activa cuando el bus quiere realizar una operación (bus_read o bus_write = '1') y la dirección está en el rango
addr_in_range <= '1' when (Bus_Addr(31 downto 8) = "000100000000000000000000") AND ((bus_Read ='1')or (bus_Write = '1'))else '0'; 


---------------------------------------------------------------------------
-- Registro que almacena las señales de control del bus
---------------------------------------------------------------------------

Read_Write_register: process (clk)
   begin
      if (clk'event and clk = '1') then
         if (reset = '1') then
            Internal_read <= '0';
				Internal_write <= '0';
         elsif load_control = '1' then 
            Internal_read <= bus_Read;
				Internal_write <= bus_write;
         end if;        
      end if;
   end process;

---------------------------------------------------------------------------
-- Memoria de datos de 64 palabras de 32 bits
---------------------------------------------------------------------------

MD_scratch: RAM_64_32 PORT MAP (CLK => CLK, enable => MD_Scratch_enable, ADDR => MD_scratch_addr, Din => Bus_Data, WE =>  Scratch_WE, RE => Scratch_RE, Dout => MD_Scratch_Dout_int);

--La salida de la scratch se guarda en un registro
output_reg: reg generic map (size => 32)
				PORT MAP(	Din => MD_Scratch_Dout_int, clk => clk, reset => reset, load => load_reg, Dout => MD_Dout);
-- registro para almacenar la @(es de 7 bits aunque para esta memoria basta con 6)
reg_addr: reg 	generic map (size => 7)
				PORT MAP(Din => Bus_Addr(8 downto 2), CLK => CLK, reset => reset, load => load_addr, Dout => addr_reg);
-- @ para la Scratch (en esta memoria son sólo 6 bits)
-- sólo asignamos los bits que se usan. El resto se quedan a 0.
MD_scratch_addr(7 downto 2) <= 	addr_reg(5 downto 0); 
MD_scratch_addr(1 downto 0) <= "00";
MD_scratch_addr(31 downto 8) <= "000000000000000000000000";


---------------------------------------------------------------------------
-- Máquina de estados para de la memoria scratch
---------------------------------------------------------------------------

SYNC_PROC: process (clk)
   begin
      if (clk'event and clk = '1') then
         if (reset = '1') then
            state <= Inicio;
         else
            state <= next_state;
         end if;        
      end if;
   end process;
--Mealy State-Machine - Outputs based on state and inputs
----------------------------------------------------------------------------
-- Esta máquina de estados gestiona load_reg, load_control, MD_Bus_TRDY, y MD_send_data
---------------------------------------------------------------------------


   OUTPUT_DECODE: process (state, addr_in_range, Internal_read, Internal_write)
   begin
		-- valores por defecto, si no se asigna otro valor en un estado valdrán lo que se asigna aquí
		
		MD_Bus_DEVsel <= '0';
		MD_Scratch_enable <= '0';
		MD_Bus_TRDY <= '0';
		MD_send_data <= '0';
		load_reg <= '0';	
		load_addr <= '0';
		load_control <= '0';
		Scratch_RE <= '0';
		Scratch_WE <= '0';
		next_state <= Inicio;		--Por defecto nos quedamos en inicio
		-- Estado inicial: Espera   
		case state is
			when Inicio =>   
				-- si la dirección no está en rango no hay que hacer nada
				If (Addr_in_range= '0') or (Bus_Frame = '0') then -- si no piden nada no hacemos nada. Sólo leemos la dirección si se activa Bus_frame
					next_state <= Inicio;
				else -- si frame es '1' y detectamos que la dirección nos pertenece la guardamos
					next_state <= Acceso;
					MD_Bus_DEVsel <= '1';
					load_addr <= '1';
					load_control <= '1';
				end if;
		   when Acceso =>     
				if (Bus_Frame = '1') then
					MD_Scratch_enable <= '1';
					IF (Internal_write = '1') then -- la escritura se hace en el ciclo actual
						MD_Bus_TRDY <= '1';
						next_state <= Inicio;
						Scratch_WE <= '1';
					ELSIF (Internal_read = '1') then
						next_state <= Envio;
						load_reg <= '1'; --La salida de la memoria se carga en el registro para enviarla en el ciclo siguiente
						Scratch_RE <= '1';
					else --No debería pasar, pero si no hay que hacer nada nos volvemos a Inicio
						next_state <= Inicio;
					end if;
				else --Si han bajado frame no hacemos nada
					next_state <= Inicio;
				end if;
			when Envio =>   
				if (Bus_Frame = '1') then	
					MD_Bus_TRDY <= '1';
					MD_send_data <= '1';
					next_state <= Inicio;	
				else --Si han bajado frame no hacemos nada
					next_state <= Inicio;
				end if;	
			when others => next_state <= Inicio;--No deberia pasar nunca
		end case;
	end process;   

end Behavioral;

