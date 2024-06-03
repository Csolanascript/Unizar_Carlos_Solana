-- TestBench Template 

  LIBRARY ieee;
  USE ieee.std_logic_1164.ALL;
  USE ieee.numeric_std.ALL;

  ENTITY testbench IS
  END testbench;

  ARCHITECTURE behavior OF testbench IS 

  -- Component Declaration
	COMPONENT AOC2_SoC is
		Port ( 	
			clk : in  STD_LOGIC;
           	reset : in  STD_LOGIC;
           	EXT_IRQ	: 	in  STD_LOGIC; 
           	INT_ACK	: 	out  STD_LOGIC;  --Signal to acknowledge the interrupt
           	IO_input: in STD_LOGIC_VECTOR (31 downto 0); -- 32 bits de entrada para el MIPS para IO
	   		IO_output : out  STD_LOGIC_VECTOR (31 downto 0)); -- 32 bits de salida para el MIPS para IO
	END COMPONENT;

          SIGNAL clk, reset, EXT_IRQ, INT_ACK :  std_logic;
          SIGNAL IO_output,IO_input  :  std_logic_vector(31 downto 0);
          
  -- Clock period definitions
   constant CLK_period : time := 10 ns;
  BEGIN

  -- Component Instantiation
   uut: AOC2_SoC PORT MAP(clk => clk, reset => reset, EXT_IRQ => EXT_IRQ, INT_ACK=> INT_ACK, IO_input => IO_input, IO_output => IO_output);

-- Clock process definitions
   CLK_process :process
   begin
		CLK <= '0';
		wait for CLK_period/2;
		CLK <= '1';
		wait for CLK_period/2;
   end process;

 stim_proc: process
   begin		
      	EXT_IRQ <= '0';
   		--No estamos usando esta entrada
      	IO_input <= x"00000000";
   		reset <= '1';
    	wait for CLK_period*2;
		reset <= '0';
-- Es el mismo testbench que en la P1, pero con la parte de las IRQs, comentada.
-- Se puede volver a activar una vez que se haya comprobado que la nueva jerarquía funciona
-- 		wait for CLK_period*10;
-- 		-- Vamos a interrumpir en momentos distintos
-- 		EXT_IRQ <= '1';
-- 		if INT_ACK = '0' then 
-- 			wait until INT_ACK ='1'; 
-- 	  	end if;
-- 		EXT_IRQ <= '0';
-- 		wait for CLK_period*40;
-- 		EXT_IRQ <= '1';
-- 		if INT_ACK = '0' then 
-- 			wait until INT_ACK ='1'; 
-- 	  	end if;
-- 		EXT_IRQ <= '0';
-- 		wait for CLK_period*50;
-- 		EXT_IRQ <= '1';
-- 		EXT_IRQ <= '1';
-- 		if INT_ACK = '0' then 
-- 			wait until INT_ACK ='1'; 
-- 	  	end if;
-- 	  	EXT_IRQ <= '0';
-- 		wait for CLK_period*50;
-- 		-- Ahora interrumpimos sin parar
-- 		EXT_IRQ <= '1';
		wait;
   end process;

  END;
