----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    11:03:23 04/03/2014 
-- Design Name: 
-- Module Name:    BR - Behavioral 
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
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use ieee.numeric_std.all;


-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
-- use IEEE.NUMERIC_STD.ALL;
-- librería para usar la función conv_integer
use IEEE.std_logic_unsigned.all;


-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity FIFO_reg is
port (
        clk : in std_logic;
		reset : in std_logic;
        cjto : in std_logic_vector (1 downto 0); --Dir del cjto reemplazado
        new_block : in std_logic;		-- Indica que hay un reemplazo y por tanto hay que actualizar la info del fifo del conjunto correspondiente				        
        via_2_rpl : out std_logic
            );
end FIFO_reg;

architecture Behavioral of FIFO_reg is
-- en el banco hay 4 registros de un bit
    type reg_array is array (0 to 3) of std_logic;
    signal reg_file : reg_array;
    
    begin 
	 process(clk)
        begin 
			if (clk'event and clk='1') then -- se escribe en flanco de subida. 
				--if RegWrite is 1, write BusW data in register RW
            	if reset='1' then 	
				    for i in 0 to 3 loop
						reg_file(i) <= '0';
					 end loop;
				else
					if (new_block = '1') then
						-- si hay un reemplazo negamos el bit que indica el cjto más antiguo (ya que habrá sido reemplazado)
						reg_file(conv_integer(cjto)) <= not(reg_file(conv_integer(cjto))) ; --forma super compacta de vhdl para hacer el decodificador y la escritura en el banco de registros
					end if;
				end if;
			end if;
    end process;
    
    --get FIFO data for the current set (conjunto)
    via_2_rpl <= reg_file(conv_integer(cjto)); -- esto es una forma muy rápida de hacer un Mux en vhdl
    
end Behavioral;

