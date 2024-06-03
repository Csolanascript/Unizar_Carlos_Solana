----------------------------------------------------------------------------------
-- Authors: Mateo Lorente, Diego  --- 873338
--			Solana Melero, Carlos --- 872815
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

entity UC is
    Port ( valid_I_ID : in  STD_LOGIC; --indica si es una instrucci�n v�lida
    	   IR_op_code : in  STD_LOGIC_VECTOR (5 downto 0);
           Branch : out  STD_LOGIC;
           RegDst : out  STD_LOGIC;
           ALUSrc : out  STD_LOGIC;
		   MemWrite : out  STD_LOGIC;
           MemRead : out  STD_LOGIC;
           MemtoReg : out  STD_LOGIC;
           RegWrite : out  STD_LOGIC;
           -- Se�ales Pr�ctica 3
		   jal : out  STD_LOGIC; --indica que es una instrucci�n jal 
           ret : out  STD_LOGIC; --indica que es una instrucci�n ret
		   undef: out STD_LOGIC; --indica que el c�digo de operaci�n no pertenence a una instrucci�n conocida. En este procesador se usa s�lo para depurar
           -- Nuevas se�ales
		   RTE	: out  STD_LOGIC -- indica que es una instrucci�n RTE	   
		   -- Fin Nuevas se�ales
		   );
end UC;

architecture Behavioral of UC is
-- Constantes para mejorar la legibilidad del c�digo
CONSTANT NOP_opcode : STD_LOGIC_VECTOR (5 downto 0) := "000000";
CONSTANT ARIT_opcode : STD_LOGIC_VECTOR (5 downto 0) := "000001";
CONSTANT LW_opcode : STD_LOGIC_VECTOR (5 downto 0) := "000010";
CONSTANT SW_opcode : STD_LOGIC_VECTOR (5 downto 0) := "000011";
CONSTANT BEQ_opcode : STD_LOGIC_VECTOR (5 downto 0) := "000100";
CONSTANT JAL_opcode : STD_LOGIC_VECTOR (5 downto 0) := "000101";
CONSTANT RET_opcode : STD_LOGIC_VECTOR (5 downto 0) := "000110";
CONSTANT RTE_opcode : STD_LOGIC_VECTOR (5 downto 0) := "001000";
begin
-- Si IR_op = 0 es nop, IR_op=1 es aritm�tica, IR_op=2 es LW, IR_op=3 es SW, IR_op= 4 es BEQ, IR_op=5 es jal, IR_op= 6 es ret, IR_op= 8 es RTE

UC_mux : process (IR_op_code, valid_I_ID)
begin 
	-- Por defecto ponemos todas las se�ales a 0 que es el valor que garantiza que no alteramos nada
	Branch <= '0'; RegDst <= '0'; ALUSrc <= '0'; MemWrite <= '0'; MemRead <= '0'; MemtoReg <= '0'; RegWrite <= '0'; UNDEF <= '0';
	jal <= '0'; ret <= '0'; RTE <= '0'; 
	IF valid_I_ID = '1' then --si la instrucci�n es v�lida analizamos su c�digo de operaci�n
		CASE IR_op_code IS
			--NOP 
			WHEN  NOP_opcode  	=>  
			--ARIT
			WHEN  ARIT_opcode  	=> 	RegDst <= '1'; RegWrite <= '1'; 
			--LW
			WHEN  LW_opcode  	=>  ALUSrc <= '1'; MemRead <= '1'; MemtoReg <= '1'; RegWrite <= '1'; 
			--SW
			WHEN  SW_opcode  	=>  ALUSrc <= '1'; MemWrite <= '1'; 
			--BEQ
			WHEN  BEQ_opcode  	=>  Branch <= '1'; 
			------------------------------------------------
			-- COMPLETAR
			------------------------------------------------
			-- JAL
			WHEN  jal_opcode  	=>  jal <= '1'; RegWrite <= '1'; 
			-- RET
			WHEN  RET_opcode  	=>  ret <= '1'; --�qu� m�s se�ales?
			--RTE
			WHEN  RTE_opcode  	=>  RTE <= '1'; --�qu� m�s se�ales?
			-- OP code undefined
			WHEN  OTHERS 	  	=> UNDEF <= '1'; --Se activa si la instrucci�n no pertenece al repertorio
		  END CASE;
	END IF;
end process;
end Behavioral;

