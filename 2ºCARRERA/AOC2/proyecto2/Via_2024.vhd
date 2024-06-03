library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all; -- se usa para convertir std_logic a enteros
entity Via is 
 	generic ( num_via: integer); -- se usa para los mensajes. Hay que poner el número correcto al instanciarla
 	port (	CLK : in std_logic;
			reset : in  STD_LOGIC;
 			Dir_word: in std_logic_vector(1 downto 0); -- se usa para elegir la palabra a la que se accede en un conjunto la cache de datos. 
 			Dir_cjto: in std_logic_vector(1 downto 0); -- se usa para elegir el conjunto
 			Tag: in std_logic_vector(25 downto 0);
 			Din : in std_logic_vector (31 downto 0);
			WE : in  STD_LOGIC; 	-- write enable	
			Tags_WE : in  STD_LOGIC; 	-- write enable para la memoria de etiquetas 
			hit : out STD_LOGIC; -- indica si es acierto
			Dout : out std_logic_vector (31 downto 0); 
			-- Gestión de los bloques sucios
			Dirty_block_addr: out std_logic_vector (31 downto 0);--@del bloque sucio
			Update_dirty	: in  STD_LOGIC; --indica que hay que actualizar el bit dirty
			dirty_bit : out  STD_LOGIC; --avisa si el bloque a reemplazar es sucio
			Block_copied_back	: in  STD_LOGIC -- indica que se ha mandado un bloque sucio a memoria. Se usa para elegir la máscara que quita el bit de sucio
			) ;
end Via;
 			
Architecture Behavioral of Via is

component reg is
    generic (size: natural := 32);  -- por defecto son de 32 bits, pero se puede usar cualquier tamaño
	Port ( Din : in  STD_LOGIC_VECTOR (size -1 downto 0);
           clk : in  STD_LOGIC;
		   reset : in  STD_LOGIC;
           load : in  STD_LOGIC;
           Dout : out  STD_LOGIC_VECTOR (size -1 downto 0));
end component;

-- definimos la memoria de contenidos de la cache de datos como un array de 16 palabras de 32 bits
type Ram_MC_data is array(0 to 15) of std_logic_vector(31 downto 0);
signal MC_data : Ram_MC_data := (  		X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", -- posiciones 0,1,2,3,4,5,6,7
									X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000");									
-- definimos la memoria de etiquetas de la cache de datos como un array de 4 palabras de 26 bits
type Ram_MC_Tags is array(0 to 3) of std_logic_vector(25 downto 0);
signal MC_Tags : Ram_MC_Tags := (  		"00000000000000000000000000", "00000000000000000000000000", "00000000000000000000000000", "00000000000000000000000000");												
signal valid_bits_in, valid_bits_out, mask: std_logic_vector(3 downto 0); -- se usa para saber si un bloque tiene info válida. Cada bit representa un bloque.									
signal valid_bit: std_logic;
signal Dir_MC: std_logic_vector(3 downto 0); -- se usa para leer/escribir las datos almacenas en al MC. 
signal MC_Tags_Dout: std_logic_vector(25 downto 0); 
-- Gestión de los bloques sucios
signal dirty_bits_in, dirty_bits_out, set_dirty_mask, set_clean_mask: std_logic_vector(3 downto 0); 
begin 
-------------------------------------------------------------------------------------------------- 
-----memoria_cache_D: memoria RAM que almacena los 4 bloques de 4 datos que puede guardar la Cache
-------------------------------------------------------------------------------------------------- 
Dir_MC <= Dir_cjto&Dir_word;
 memoria_cache_D: process (CLK)
    begin
        if (CLK'event and CLK = '1') then
            if (WE = '1') then -- sólo se escribe si WE vale 1
                MC_data(conv_integer(Dir_MC)) <= Din;
				-- report saca un mensaje en la consola del simulador.  Nos imforma sobre qué dato se ha escrito, dónde y cuándo
				report "Simulation time : " & time'IMAGE(now) & ".  Data written in via " & integer'image(num_via) & ": " & integer'image(to_integer(unsigned(Din))) & ", in Dir_cjto = " & integer'image(to_integer(unsigned(Dir_cjto)));
            end if;
        end if;
    end process;
    Dout <= MC_data(conv_integer(Dir_MC)); 
-------------------------------------------------------------------------------------------------- 
-----MC_Tags: memoria RAM que almacena las 4 etiquetas
-------------------------------------------------------------------------------------------------- 
memoria_cache_tags: process (CLK)
    begin
        if (CLK'event and CLK = '1') then
            if (Tags_WE = '1') then -- sólo se escribe si Tags_WE vale 1
                MC_Tags(conv_integer(dir_cjto)) <= Tag;
				-- report saca un mensaje en la consola del simulador. Nos imforma sobre qué etiqueta se ha escrito, dónde y cuándo
				report "Simulation time : " & time'IMAGE(now) & ".  Tag written in via " & integer'image(num_via) & ": " & integer'image(to_integer(unsigned(Tag))) & ", in Dir_cjto = " & integer'image(to_integer(unsigned(dir_cjto)));
            end if;
        end if;
    end process;
    MC_Tags_Dout <= MC_Tags(conv_integer(dir_cjto)); 
-------------------------------------------------------------------------------------------------- 
-- registro de validez. Al resetear los bits de validez se ponen a 0 así evitamos falsos positivos por basura en las memorias
-- en el bit de validez se escribe a la vez que en la memoria de etiquetas. Hay que poner a 1 el bit que toque y mantener los demás, para eso usamos una mascara generada por un decodificador
-------------------------------------------------------------------------------------------------- 
mask			<= 	"0001" when dir_cjto="00" else
						"0010" when dir_cjto="01" else
						"0100" when dir_cjto="10" else
						"1000" when dir_cjto="11" else
						"0000";
valid_bits_in <= valid_bits_out OR mask;
bits_validez: reg generic map (size => 4)	port map(	Din => valid_bits_in, clk => clk, reset => reset, load => Tags_WE, Dout => valid_bits_out);
-------------------------------------------------------------------------------------------------- 
valid_bit <= 	valid_bits_out(0) when dir_cjto="00" else
						valid_bits_out(1) when dir_cjto="01" else
						valid_bits_out(2) when dir_cjto="10" else
						valid_bits_out(3) when dir_cjto="11" else
						'0';
-------------------------------------------------------------------------------------------------- 
-- registro de bloques sucios. Al resetear los bits de sucio se ponen a 0. Es decir se pierde la información que hay en la MC 
-- Nota debería haber una entrada flush: para vaciar la MC. De forma que todos los bloques sucios se actualizasen en memoria
-------------------------------------------------------------------------------------------------- 
bits_dirty: reg generic map (size => 4)	port map(	Din => dirty_bits_in, clk => clk, reset => reset, load => Update_dirty, Dout => dirty_bits_out);
set_dirty_mask 	<= mask OR dirty_bits_out; --Para marcar el cjto actual como sucio
set_clean_mask 	<= Not(mask) AND dirty_bits_out; --Para marcar el cjto actual como limpio
dirty_bits_in 	<= set_clean_mask when Block_copied_back ='1' else set_dirty_mask; --cuando se reemplaza el bloque hay que marcar el cjto como limpio. Cuando se escribe hay que marcarlo como sucio
dirty_bit 		<= 	dirty_bits_out(0) when dir_cjto="00" else
					dirty_bits_out(1) when dir_cjto="01" else
					dirty_bits_out(2) when dir_cjto="10" else
					dirty_bits_out(3) when dir_cjto="11" else
					'0';
-------------------------------------------------------------------------------------------------- 
-- Señal de hit: se activa cuando la etiqueta coincide y el bit de valido es 1
hit <= '1' when ((MC_Tags_Dout= Tag) AND (valid_bit='1'))else '0'; --comparador que compara el tag almacenado en MC con el de la dirección y si es el mismo y el bloque tiene el bit de válido activo devuelve un 1
-------------------------------------------------------------------------------------------------- 
-- @ del bloque sucio por si hay que hacer copy-back
Dirty_block_addr <= MC_Tags_Dout&dir_cjto&"0000"; --Si es fallo mandamos la dirección del bloque que causó el fallo, si es copy-back la del bloque reemplazado
end Behavioral;