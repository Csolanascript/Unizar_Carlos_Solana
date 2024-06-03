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


entity MD_cont is port (
		  CLK : in std_logic;
		  reset: in std_logic;
		  Bus_Frame: in std_logic; -- indica que el master quiere más datos
		  bus_last_word : in  STD_LOGIC; --indica que es el último dato de la transferencia
		  bus_Read: in std_logic;
		  bus_Write: in std_logic;
		  Bus_Addr : in std_logic_vector (31 downto 0); --Direcciones 
		  Bus_Data : in std_logic_vector (31 downto 0); --Datos  
		  MD_Bus_DEVsel: out std_logic; -- para avisar de que se ha reconocido que la dirección pertenece a este módulo
		  MD_Bus_TRDY: out std_logic; -- para avisar de que se va a realizar la operación solicitada en el ciclo actual
		  MD_send_data: out std_logic; -- para enviar los datos al bus
          MD_Dout : out std_logic_vector (31 downto 0)		  -- salida de datos
		  );
end MD_cont;

architecture Behavioral of MD_cont is

component counter is
 	generic (   size : integer := 10);
	Port ( clk : in  STD_LOGIC;
       reset : in  STD_LOGIC;
       count_enable : in  STD_LOGIC;
       count : out  STD_LOGIC_VECTOR (size-1 downto 0));
end component;

-- misma memoria que en el proyecto anterior
component RAM_128_32 is port (
		  CLK : in std_logic;
		  enable: in std_logic; --solo se lee o escribe si enable está activado
		  ADDR : in std_logic_vector (31 downto 0); --Dir 
          Din : in std_logic_vector (31 downto 0);--entrada de datos para el puerto de escritura
          WE : in std_logic;		-- write enable	
		  RE : in std_logic;		-- read enable		  
		  Dout : out std_logic_vector (31 downto 0));
end component;

component reg is
    generic (size: natural := 32);  -- por defecto son de 32 bits, pero se puede usar cualquier tamaño
	Port ( Din : in  STD_LOGIC_VECTOR (size -1 downto 0);
           clk : in  STD_LOGIC;
		   reset : in  STD_LOGIC;
           load : in  STD_LOGIC;
           Dout : out  STD_LOGIC_VECTOR (size -1 downto 0));
end component;


signal BUS_RE, BUS_WE, MEM_WE, contar_palabras, reset_cuenta, reset_cont_retardos, MD_enable, memoria_preparada, contar_retardos, direccion_distinta, reset_retardo, load_addr, Addr_in_range: std_logic;
signal addr_frame, last_addr:  STD_LOGIC_VECTOR (6 downto 0);
signal cuenta_palabras, cuenta_retardos:  STD_LOGIC_VECTOR (7 downto 0);
signal MD_addr: STD_LOGIC_VECTOR (31 downto 0);
type state_type is (Espera, Transferencia, Detectado); 
signal state, next_state : state_type; 
signal last_addr_valid_vector: std_logic_vector (0 downto 0);--indica si el registo last_addr tiene una dirección válida y no un 0 proveniente de un reset
signal load_control, Internal_read, Internal_write, last_addr_valid: std_logic; --signals to store inputs bus_read, and bus_write
begin
---------------------------------------------------------------------------
-- Decodificador: identifica cuando la dirección pertenece a la MD: (X"00000000"-X"000001FF")
---------------------------------------------------------------------------
-- Se activa cuando el bus quiere realizar una operación (bus_read o bus_write = '1') y la dirección está en el rango
Addr_in_range <= '1' when (Bus_Addr(31 downto 9) = "00000000000000000000000") AND ((bus_Read ='1')or (bus_Write = '1')) else '0'; 

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

BUS_RE <= Internal_read;

BUS_WE <= Internal_write;

---------------------------------------------------------------------------
-- HW para introducir retardos:
-- Con un contador y una sencilla máquina de estados introducimos un retardo en la memoria de forma articial. 
-- Cuando se pide una dirección nueva manda la primera palabra en 4 ciclos y el resto cada dos
-- Si se accede dos veces a la misma dirección la segunda vez no hay retardo inicial
---------------------------------------------------------------------------
reset_cont_retardos <= reset or reset_retardo;
cont_retardos: counter 		generic map (size => 8)	port map (clk => clk, reset => reset_retardo, count_enable => contar_retardos, count => cuenta_retardos);

-- este registro almacena la ultima dirección accedida. Cada vez que cambia la dirección se resetea el contador de retaros
-- La idea es simular que cuando accedes a una dirección nueva tarda más. Si siempre accedes a la misma no introducirá retardos adicionales
reg_last_addr: reg 	generic map (size => 7)
					PORT MAP(Din => Bus_Addr(8 downto 2), CLK => CLK, reset => reset, load => load_addr, Dout => last_addr);
reg_last_addr_valid: reg 	generic map (size => 1)
							PORT MAP(Din => "1", CLK => CLK, reset => reset, load => load_addr, Dout => last_addr_valid_vector);
last_addr_valid <= 	last_addr_valid_vector(0); -- conversión de tipos						
direccion_distinta <= '0' when ((last_addr= Bus_Addr(8 downto 2)) and (last_addr_valid='1')) else '1';
--introducimos un retardo en la memoria de forma articial. Manda la primera palabra en el cuarto ciclo y el resto cada dos ciclos
-- Pero si los accesos son a direcciones repetidas el retardo inicial desaparece

memoria_preparada <= '0' when (cuenta_retardos < "00000011" or cuenta_retardos(0) = '1') else '1';
---------------------------------------------------------------------------
-- Máquina de estados para gestionar las transferencias e introducir retardos
---------------------------------------------------------------------------

SYNC_PROC: process (clk)
   begin
      if (clk'event and clk = '1') then
         if (reset = '1') then
            state <= Espera;
         else
            state <= next_state;
         end if;        
      end if;
   end process;


   
 --MEALY State-Machine - Outputs based on state and inputs
   OUTPUT_DECODE: process (state, direccion_distinta, Addr_in_range, memoria_preparada, Bus_Frame, reset_cuenta)
   begin
		-- valores por defecto, si no se asigna otro valor en un estado valdrán lo que se asigna aquí
		contar_retardos <= '0';
		reset_retardo <= '0';
		load_addr <= '0';
		load_control <= '0';
		next_state <= Espera;
		MD_Bus_DEVsel <= '0';
		MD_Bus_TRDY <= '0'; 
		MD_send_data <= '0';
		MEM_WE <= '0';
		MD_enable <= '0';
		contar_palabras <= '0';
		
		  
		case state is
			-- Estado inicial: Espera 
			when Espera =>   
				If (Addr_in_range= '0') or (Bus_Frame = '0') then -- si no piden nada no hacemos nada. Sólo leemos la dirección si se activa Bus_frame
					next_state <= Espera;
				else  -- si detectamos que hay una transferencia y la dirección nos pertenece vamos al estado de transferencia
					next_state <= Detectado;
					MD_Bus_DEVsel <= '1'; -- avisamos de que hemos visto que la dirección es nuestra
					load_control <= '1'; -- Para cargar las señales de Bus_read y Bus_write
					IF (direccion_distinta ='1') then
						reset_retardo <= '1'; -- si se repite la dirección no metemos los retardos iniciales
						load_addr <= '1'; --cargamos  la dirección 
					end if;	
				end if;	
   	    -- Estado Detectado: sirve para informar de que hemos visto que la dirección es nuestra y de que vamos a empezar a leer/escribir datos 
      		when Detectado =>   
				If (Bus_Frame = '1') then
					next_state <= Transferencia;
					MD_Bus_DEVsel <= '1'; -- avisamos de que hemos visto que la dirección es nuestra
				  -- No empezamos a leer/escribir por si acaso no mandan los datos hasta el ciclo siguiente
				else 	--Cuando Bus_Frame es 0 es que hemos terminado. No debería pasar porque todavía no hemos hecho nada
					next_state <= Espera;
				end if;
		  -- Estado Transferencia
			when Transferencia =>   
				if (Bus_Frame = '1') then -- si estamos en una transferencia seguimos enviando/recibiendo datos hasta que el master diga que no quiere más
					MD_Bus_DEVsel <= '1'; -- avisamos de que hemos visto que la dirección es nuestra
					MD_enable <= '1'; --habilitamos la MD para leer o escribir
					contar_retardos <= '1'; 
					MD_Bus_TRDY <= memoria_preparada;
					contar_palabras <= memoria_preparada; -- cada vez que mandamos una palabra se incrementa el contador
					MEM_WE <= Bus_WE and memoria_preparada; --evitamos escribir varias veces
					MD_send_data <= Bus_RE AND memoria_preparada; -- si la dirección está en rango y es una lectura se carga el dato de MD en el bus
					if ((bus_last_word='1')and(memoria_preparada = '1')) then --si estamos enviando la última palabra hemos terminado
						next_state <= Espera;
						reset_retardo <= '1';
					else
						next_state <= Transferencia;
					end if;
				else  --no debería pasar. Si pasa quiere decir que han desactivado el frame sin poner last_word
					next_state <= Transferencia;
					next_state <= Espera;
				end if;	
		end case;
	end process;

---------------------------------------------------------------------------
-- calculo direcciones 
-- el contador cuenta mientras frame esté activo, la dirección pertenezca a la memoria y la memoria esté preparada para realizar la operación actual. 
---------------------------------------------------------------------------

-- Para que este esquema funcione hay que avisar cuando se pide la última palabra. Al enviarla se resetea la cuenta de la ráfaga, y así la siguiente ráfaga empezara por la dirección inicial
reset_cuenta <= '1' when ((reset='1') or ((bus_last_word='1') and (memoria_preparada = '1'))) else '0';
cont_palabras: counter 		generic map (size => 8) port map (clk => clk, reset => reset_cuenta, count_enable => contar_palabras, count => cuenta_palabras);
-- La dirección se calcula sumando la cuenta de palabras a la dirección inicial almacenada en el registro last_addr
addr_Frame <= 	last_addr + cuenta_palabras(6 downto 0);
-- sólo asignamos los bits que se usan. El resto se quedan a 0.
MD_addr(8 downto 2) <= 	addr_Frame; 
MD_addr(1 downto 0) <= "00";
MD_addr(31 downto 9) <= "00000000000000000000000";

---------------------------------------------------------------------------
-- Memoria de datos original 
---------------------------------------------------------------------------


MD: RAM_128_32 PORT MAP (CLK => CLK, enable => MD_enable, ADDR => MD_addr, Din => Bus_Data, WE =>  MEM_WE, RE => Bus_RE, Dout => MD_Dout);


end Behavioral;

