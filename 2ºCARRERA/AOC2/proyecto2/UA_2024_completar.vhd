----------------------------------------------------------------------------------
-- Authors: Mateo Lorente, Diego  --- 873338
--			Solana Melero, Carlos --- 872815
----------------------------------------------------------------------------------


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity UA is
	Port(
			valid_I_MEM : in  STD_LOGIC; --indica si es una instrucci�n de MEM es v�lida
			valid_I_WB : in  STD_LOGIC; --indica si es una instrucci�n de WB es v�lida
			Reg_Rs_EX: IN  std_logic_vector(4 downto 0); 
			Reg_Rt_EX: IN  std_logic_vector(4 downto 0);
			RegWrite_MEM: IN std_logic;
			RW_MEM: IN  std_logic_vector(4 downto 0);
			RegWrite_WB: IN std_logic;
			RW_WB: IN  std_logic_vector(4 downto 0);
			MUX_ctrl_A: out std_logic_vector(1 downto 0);
			MUX_ctrl_B: out std_logic_vector(1 downto 0)
		);
	end UA;

Architecture Behavioral of UA is
signal Corto_A_Mem, Corto_B_Mem, Corto_A_WB, Corto_B_WB: std_logic;
begin

-- Dise�o incompleto. Os lo ponemos c�mo ejemplo. Deb�is completarlo vosotros
-- Activamos la se�al corto_A_Mem, cuand detectamos que el operando almacenado en A (Rs) es el mismo en el que va a escribir la instrucci�n que est� en la etapa Mem
-- Importante: s�lo activamos el corto si la instrucci�n de la etapa MEM en v�lida
-- IMPORTANTE: la instrucci�n JAL es algo especial porque el dato que escribe en BR es distinto al resto
-- �Se puede usar la red de anticipaci�n en los casos JAL-uso a distancia 1 y 2? �Hay alg�n caso que no?�Puede ocurrir en este procesador?�Qu� soluci�n propon�is?
-- Ejemplo: Jal r1, @salto; @salto: ADD R1, R2, R1; 
	Corto_A_Mem <= '1' when ((Reg_Rs_EX = RW_MEM) and (RegWrite_MEM = '1') and (valid_I_MEM = '1'))	else '0';
	-- Completar: Resto de cortos:
	Corto_B_Mem <= '1' when ((Reg_Rt_EX = RW_MEM) and (RegWrite_MEM = '1') and (valid_I_MEM = '1'))	else '0';
	Corto_A_WB	<= '1' when ((Reg_Rs_EX = RW_WB) and (RegWrite_WB = '1') and (valid_I_WB = '1'))	else '0';
	Corto_B_WB	<= '1' when ((Reg_Rt_EX = RW_WB) and (RegWrite_WB = '1') and (valid_I_WB = '1'))	else '0';
	-- Con las se�ales anteriores se elige la entrada de los muxes:
	-- entrada 00: se corresponde al dato del banco de registros
	-- entrada 01: dato de la etapa Mem
	-- entrada 10: dato de la etapa WB
	-- Completar: Ponemos un ejemplo para el Corto_A_Mem, deb�is a�adir el resto de casos
	MUX_ctrl_A <= 	"01" when (Corto_A_Mem = '1') else
					"10" when (Corto_A_WB = '1') else
					"00";
	MUX_ctrl_B <= 	"01" when (Corto_B_Mem = '1') else
					"10" when (Corto_B_WB = '1') else
					"00";
end Behavioral;


--Preguna: Si, porque dichas instrucciones se ejecutarian en el monento de retorno, y si tiene problemas de anticipacion ya se gestionan en esta UA. No habria que hacerlo