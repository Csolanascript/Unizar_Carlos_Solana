library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
-- Este módulo monitoriza una entrada del sistema y la escribe a través del bus en una dirección de memoria de la Scrtach
-- EN la versión actual intenta escribir siempre (IO_M_Req ='1') el dato que recibe (IO_input) que representa una entrada del sistema. 
-- Lo escribe en la palabra 0 de la scratch. De esta forma el procesador puede acceder al valor de la entrada 
entity IO_master is
    Port ( 	clk: in  STD_LOGIC; 
		    reset: in  STD_LOGIC; 
			IO_M_bus_Grant: in std_logic; 
			IO_input: in STD_LOGIC_VECTOR (31 downto 0);
			bus_TRDY : in  STD_LOGIC; --indica que el esclavo no puede realizar la operación solicitada en este ciclo
			Bus_DevSel: in  STD_LOGIC; --indica que el esclavo ha reconocido que la dirección está dentro de su rango
			IO_M_ERROR: out std_logic; -- Se activa si el esclavo no responde a su dirección
			IO_M_Req: out std_logic; 
			IO_M_Read: out std_logic; 
			IO_M_Write: out std_logic;
			IO_M_bus_Frame: out std_logic; 
			IO_M_send_Addr: out std_logic;
			IO_M_send_Data: out std_logic;
			IO_M_last_word: out std_logic;
			IO_M_Addr: out STD_LOGIC_VECTOR (31 downto 0);
			IO_M_Data: out STD_LOGIC_VECTOR (31 downto 0)); 
end IO_master;
Architecture Behavioral of IO_master is

type state_type is (Inicio, send_addr, send_data); 
type error_type is (addr_error, No_error); 
signal state, next_state : state_type; 
signal error_state, next_error_state : error_type; 
signal internal_IO_M_send_Addr: std_logic;
begin
-- maquina de estados del máster
---------------------------------------------------------------------------
-- Máquina de estados para de la memoria scratch
---------------------------------------------------------------------------

State_reg: process (clk)
   begin
      if (clk'event and clk = '1') then
         if (reset = '1') then
            state <= Inicio;           
         else
            state <= next_state;
         end if;        
      end if;
   end process;
   
---------------------------------------------------------------------------
-- 2023
-- Máquina de estados para el bit de error
---------------------------------------------------------------------------

Error_reg: process (clk)
   begin
      if (clk'event and clk = '1') then
         if (reset = '1') then           
            error_state <= No_error;
        else
            error_state <= next_error_state;
         end if;   
      end if;
   end process;
--Mealy State-Machine - Outputs based on state and inputs
----------------------------------------------------------------------------
-- Esta máquina de estados gestiona la escritura de la entrada IO en la última palabra de la Scratch
---------------------------------------------------------------------------


   OUTPUT_DECODE: process (state, IO_M_bus_Grant, bus_TRDY, Bus_DevSel)
   begin
		-- valores por defecto, si no se asigna otro valor en un estado valdrán lo que se asigna aquí
		
		IO_M_Req <= '0'; -- si no lo tenemos pedimos el bus
		IO_M_bus_Frame <= '0';	
		IO_M_send_Addr <= '0';	
		IO_M_send_Data <= '0';	
		IO_M_last_word <= '0';	
		IO_M_Read <= '0'; -- nunca lee
		IO_M_Write <= '0';
		IO_M_Addr <= X"10000000";	--Mandamos siempre la misma @ la dirección 0 de la scratch. De esta forma el procesador puede leer la entrada externa
		IO_M_Data <= IO_input;		-- escribe el dato de la entrada	
		next_state <= Inicio;		--Por defecto nos quedamos en inicio
		next_error_state <= error_state; -- por defecto mantenemos el valor de error
		-- Estado inicial: Espera   
		If (state = Inicio) then
			IO_M_Req <= '1';
			If (IO_M_bus_Grant= '0') then -- si no nos dan el bus no hacemos nada
				next_state <= Inicio;
      		else  -- si nos dan el bus enviamos la dirección
         		next_state <= send_addr;
      		end if;	
      	-- Ya nos han concedido el bus. Activamos frame y enviamos la dirección.  
		elsif state= send_addr then 	
      		IO_M_bus_Frame <= '1';--activamos bus_frame para deshabilitar al árbitro
			IO_M_send_Addr <= '1';
         	IO_M_Write <= '1';
			If (Bus_DevSel = '1') then --si el esclavo reconoce la dirección pasamos a la fase de datos
				next_state <= send_data;
				next_error_state <= No_error; -- marcamos que el esclavo ha reconocido la última dirección
			else
				next_error_state <= addr_error; -- marcamos que el esclavo no ha reconocido la última dirección	
				next_state <= Inicio;-- Si el esclavo no responde, damos por cerrada la transferencia
			end if;	
      	elsif state= send_data then 	
         	IO_M_send_Addr <= '0';
         	IO_M_Write <= '1';
			IO_M_bus_Frame <= '1'; --activamos bus_frame para deshabilitar al árbitro
			IO_M_send_Data <= '1'; --enviamos el dato de la entrada	
			IO_M_last_word <= '1'; --indicamos que es la última palabra			
        	If (bus_TRDY = '1') then  --si el esclavo está preparado se realiza la escritura en la scratch
				next_state <= inicio;		
			else
				next_state <= send_data;
			end if;   				
	   	end if;	
	end process;   
	
	
	IO_M_ERROR <= '1' when (error_state = addr_error) else '0';

end Behavioral;

		