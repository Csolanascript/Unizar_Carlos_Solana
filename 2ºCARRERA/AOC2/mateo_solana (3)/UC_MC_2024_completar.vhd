---------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    13:38:18 05/15/2014 
-- Design Name: 
-- Module Name:    UC_slave - Behavioral 
-- Project Name: 
-- Target Devices: 
-- Tool versions: 
-- Description: 
--
-- Dependencies: 
--
-- Revision: 
-- Revision 0.01 - File Created
-- Additional Comments: la UC incluye un contador de 2 bits para llevar la cuenta de las transferencias de bloque y una máquina de estados
--
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity UC_MC is
    Port ( 	clk : in  STD_LOGIC;
			reset : in  STD_LOGIC;
			-- Órdenes del MIPS
			RE : in  STD_LOGIC; 
			WE : in  STD_LOGIC;
			-- Respuesta al MIPS
			ready : out  STD_LOGIC; -- indica si podemos procesar la orden actual del MIPS en este ciclo. En caso contrario habrá que detener el MIPs
			-- Señales de la MC
			hit0 : in  STD_LOGIC; --se activa si hay acierto en la via 0
			hit1 : in  STD_LOGIC; --se activa si hay acierto en la via 1
			via_2_rpl :  in  STD_LOGIC; --indica que via se va a reemplazar
			addr_non_cacheable: in STD_LOGIC; --indica que la dirección no debe almacenarse en MC. En este caso porque pertenece a la scratch
			internal_addr: in STD_LOGIC; -- indica que la dirección solicitada es de un registro de MC
			MC_WE0 : out  STD_LOGIC;
            MC_WE1 : out  STD_LOGIC;
            MC_bus_Rd_Wr : out  STD_LOGIC; --1 para escritura en Memoria y 0 para lectura
			MC_tags_WE : out  STD_LOGIC; -- para escribir la etiqueta en la memoria de etiquetas
            palabra : out  STD_LOGIC_VECTOR (1 downto 0);--indica la palabra actual dentro de una transferencia de bloque (1ª, 2ª...)
            mux_origen: out STD_LOGIC; -- Se utiliza para elegir si el origen de la dirección de la palabra y el dato es el Mips (cuando vale 0) o la UC y el bus (cuando vale 1)
			block_addr : out  STD_LOGIC; -- indica si la dirección a enviar es la de bloque (rm) o la de palabra (w)
			mux_output: out  std_logic_vector(1 downto 0); -- para elegir si le mandamos al procesador la salida de MC (valor 0),los datos que hay en el bus (valor 1), o un registro interno( valor 2)
			-- señales para los contadores de rendimiento de la MC
			inc_m : out STD_LOGIC; -- indica que ha habido un fallo en MC
			inc_w : out STD_LOGIC; -- indica que ha habido una escritura en MC
			inc_r : out STD_LOGIC; -- indica que ha habido una escritura en MC
			inc_cb :out STD_LOGIC; -- indica que ha habido un reemplazo sucio en MC
			-- Gestión de errores
			unaligned: in STD_LOGIC; --indica que la dirección solicitada por el MIPS no está alineada
			Mem_ERROR: out std_logic; -- Se activa si en la ultima transferencia el esclavo no respondió a su dirección
			load_addr_error: out std_logic; --para controlar el registro que guarda la dirección que causó error
			-- Gestión de los bloques sucios
			send_dirty: out std_logic;-- Indica que hay que enviar la @ del bloque sucio
			Update_dirty	: out  STD_LOGIC; --indica que hay que actualizar los bits dirty tanto por que se ha realizado una escritura, como porque se ha enviado el bloque sucio a memoria
			dirty_bit : in  STD_LOGIC; --indica si el bloque a reemplazar es sucio
			Block_copied_back	: out  STD_LOGIC; -- indica que se ha enviado a memoria un bloque que estaba sucio. Se usa para elegir la máscara que quita el bit de sucio
			-- Para gestionar las transferencias a través del bus
			bus_TRDY : in  STD_LOGIC; --indica que la memoria puede realizar la operación solicitada en este ciclo
			Bus_DevSel: in  STD_LOGIC; --indica que la memoria ha reconocido que la dirección está dentro de su rango
			Bus_grant :  in  STD_LOGIC; --indica la concesión del uso del bus
			MC_send_addr_ctrl : out  STD_LOGIC; --ordena que se envíen la dirección y las señales de control al bus
            MC_send_data : out  STD_LOGIC; --ordena que se envíen los datos
            Frame : out  STD_LOGIC; --indica que la operación no ha terminado
            last_word : out  STD_LOGIC; --indica que es el último dato de la transferencia
            Bus_req :  out  STD_LOGIC --indica la petición al árbitro del uso del bus
			);
end UC_MC;

architecture Behavioral of UC_MC is
 
component counter is 
	generic (
	   size : integer := 10
	);
	Port ( clk : in  STD_LOGIC;
	       reset : in  STD_LOGIC;
	       count_enable : in  STD_LOGIC;
	       count : out  STD_LOGIC_VECTOR (size-1 downto 0)
					  );
end component;		           
-- Ejemplos de nombres de estado. No hay que usar estos. Nombrad a vuestros estados con nombres descriptivos. Así se facilita la depuración
type state_type is (Inicio, single_word_transfer_addr, read_block, write_dirty_block, single_word_transfer_data, block_transfer_addr, block_transfer_data, Send_Addr, Send_ADDR_CB, fallo, CopyBack, bajar_Frame); 
type error_type is (memory_error, No_error); 
signal state, next_state : state_type; 
signal error_state, next_error_state : error_type; 
signal last_word_block: STD_LOGIC; --se activa cuando se está pidiendo la última palabra de un bloque
signal one_word: STD_LOGIC; --se activa cuando sólo se quiere transferir una palabra
signal count_enable: STD_LOGIC; -- se activa si se ha recibido una palabra de un bloque para que se incremente el contador de palabras
signal hit: std_logic;
signal palabra_UC : STD_LOGIC_VECTOR (1 downto 0);
begin

hit <= hit0 or hit1;	
 
--el contador nos dice cuantas palabras hemos recibido. Se usa para saber cuando se termina la transferencia del bloque y para direccionar la palabra en la que se escribe el dato leido del bus en la MC
word_counter: counter 	generic map (size => 2)
						port map (clk, reset, count_enable, palabra_UC); --indica la palabra actual dentro de una transferencia de bloque (1ª, 2ª...)

last_word_block <= '1' when palabra_UC="11" else '0';--se activa cuando estamos pidiendo la última palabra

palabra <= palabra_UC;

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

error_reg: process (clk)
   begin
      if (clk'event and clk = '1') then
         if (reset = '1') then           
            error_state <= No_error;
        else
            error_state <= next_error_state;
         end if;   
      end if;
   end process;
   
--Salida Mem Error
Mem_ERROR <= '1' when (error_state = memory_error) else '0';

--Mealy State-Machine - Outputs based on state and inputs
   
   --MEALY State-Machine - Outputs based on state and inputs
   OUTPUT_DECODE: process (state, hit, last_word_block, bus_TRDY, RE, WE, Bus_DevSel, Bus_grant, via_2_rpl, hit0, hit1, addr_non_cacheable, internal_addr, unaligned)
   begin
			  -- valores por defecto, si no se asigna otro valor en un estado valdrán lo que se asigna aquí
	MC_WE0 <= '0';
	MC_WE1 <= '0';
	MC_bus_Rd_Wr <= '0';
	MC_tags_WE <= '0';
    ready <= '0';
    mux_origen <= '0';
    MC_send_addr_ctrl <= '0';
    MC_send_data <= '0';
    next_state <= state;  
	count_enable <= '0';
	Frame <= '0';
	block_addr <= '0';
	inc_m <= '0';
	inc_w <= '0';
	inc_r <= '0';
	inc_cb <= '0';
	Bus_req <= '0';
	one_word <= '0';
	mux_output <= "00";
	last_word <= '0';
	next_error_state <= error_state; 
	load_addr_error <= '0';
	send_dirty <= '0';
	Update_dirty <= '0';
	Block_copied_back <= '0';
				
        -- Estado Inicio          
    if (state = Inicio and RE= '0' and WE= '0') then -- si no piden nada no hacemos nada
		next_state <= Inicio;
		ready <= '1';
	elsif (state = Inicio) and ((RE= '1') or (WE= '1')) and  (unaligned ='1') then -- si el procesador quiere leer una dirección no alineada
		-- Se procesa el error y se ignora la solicitud
		next_state <= Inicio;
		ready <= '1';
		next_error_state <= memory_error; --última dirección incorrecta (no alineada)
		load_addr_error <= '1';
    elsif (state = Inicio and RE= '1' and  internal_addr ='1') then -- si quieren leer un registro de la MC se lo mandamos
    	next_state <= Inicio;
		ready <= '1';
		mux_output <= "00"; -- Completar. "00" es el valor por defecto. ¿Qué valor hay que poner?
		next_error_state <= No_error; --Cuando se lee el registro interno el controlador quita la señal de error
	elsif (state = Inicio and WE= '1' and  internal_addr ='1') then -- si quieren escribir en el registro interno de la MC se genera un error porque es sólo de lectura
    	next_state <= Inicio;
		ready <= '1';
		next_error_state <= memory_error; --última dirección incorrecta (intento de escritura en registro de lectura)
		load_addr_error <= '1';
	elsif (state = Inicio and RE= '1' and  hit='1') then -- si piden leer y es acierto mandamos el dato
        next_state <= Inicio;
		ready <= '1';
		inc_r <= '1'; -- se lee la MC
		mux_output <= "00"; -- Completar. "00" es el valor por defecto. ¿Qué valor hay que poner?
	elsif (state = Inicio and WE= '1' and  hit='1') then -- si piden escribir y es acierto, actualizamos MC y marcamos el bloque como sucio
        --Completar. Todas las señales están a '0' por defecto. Pensad cuales son los valores correctos
		next_state <= Inicio;
		ready <= '0';
		MC_WE0 <= '0'; --activamos la escritura en el banco en el que se ha acertado
		MC_WE1 <= '0'; 	
		mux_origen <= '0';-- la dir de la palabra viene del Mips
		Update_dirty <= '0'; --Ponemos a '1' el bit dirty
		inc_w <=  '0'; -- como la operación era de escritura incrementamos el contador
	--Completar. ¿Qué más hay que hacer en INICIO?. 
	--Completar. ¿Qué más estados tenéis?. 
------------------------------------------------------------------------------------------------------------------------
--¿Cómo desarrollar esta UC?
-- Id paso a paso. Incluid primero la gestión de los aciertos y fallos de lectura. El primero está ya casi hecho. El segundo implica pedir el bus, cuando os lo den, enviar la dirección del bloque,
-- comprobar que el server responde a la dirección, recibir las cuatro palabras a través del bus y escribirlas en MC. 
-- Cuando funcionen las lecturas vamos añadiendo funcionalidades, probándolas: fallo y acierto de esritura, reemplazo sucio, acceso a MDS en lectura y escritura, gestión del abort y acceso al registro interno de la MC
-- Os damos un banco de pruebas inicial para los fallos y aciertos de lectura. Diseñad los vuestros para el resto de casos. 	
							
	end if;
		
   end process;
 
   
end Behavioral;

