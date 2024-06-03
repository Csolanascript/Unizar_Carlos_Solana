----------------------------------------------------------------------------------
-- Authors: Mateo Lorente, Diego  --- 873338
--			Solana Melero, Carlos --- 872815
----------------------------------------------------------------------------------


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
--Mux 4 a 1
entity UD is
    Port ( 	
			valid_I_ID : in  STD_LOGIC; --indica si es una instrucci�n de ID es v�lida
			valid_I_EX : in  STD_LOGIC; --indica si es una instrucci�n de EX es v�lida
			valid_I_MEM : in  STD_LOGIC; --indica si es una instrucci�n de MEM es v�lida
    		Reg_Rs_ID: in  STD_LOGIC_VECTOR (4 downto 0); --registros Rs y Rt en la etapa ID
		  	Reg_Rt_ID	: in  STD_LOGIC_VECTOR (4 downto 0);
			MemRead_EX	: in std_logic; -- información sobre la instrucción en EX (destino, si lee de memoria y si escribe en registro)
			RegWrite_EX	: in std_logic;
			RW_EX			: in  STD_LOGIC_VECTOR (4 downto 0);
			RegWrite_Mem	: in std_logic;-- informacion sobre la instruccion en Mem (destino y si escribe en registro)
			RW_Mem			: in  STD_LOGIC_VECTOR (4 downto 0);
			IR_op_code	: in  STD_LOGIC_VECTOR (5 downto 0); -- código de operación de la instrucción en IEEE
         	salto_tomado			: in std_logic; -- 1 cuando se produce un salto 0 en caso contrario
         	--Nuevo
         	JAL_EX : in std_logic; -- Indica que la instrucci�n en EX es un JAL
         	JAL_MEM : in std_logic; -- Indica que la instrucci�n en MEM es un JAL
         	Mem_ready: in std_logic; -- 1 cuando la memoria puede realizar la operaci�n solicitada en el ciclo actual
			parar_MIPS: out  STD_LOGIC; -- Indica que hay que detener todo el procesador porque la etapa MEM no est� preparada
         	Kill_IF		: out  STD_LOGIC; -- Indica que la instrucci�n en IF no debe ejecutarse (fallo en la predicci�n de salto tomado)
			Parar_ID		: out  STD_LOGIC -- Indica que las etapas ID e IF deben parar
			); 
end UD;
Architecture Behavioral of UD is
signal dep_rs_EX, dep_rs_Mem, dep_rt_EX, dep_rt_Mem, ld_uso_rs, ld_uso_rt, JAL_uso_rs, JAL_uso_rt, RET_rs, BEQ_rs, BEQ_rt, riesgo_datos_ID : std_logic;
CONSTANT NOP : STD_LOGIC_VECTOR (5 downto 0) := "000000";
CONSTANT LW : STD_LOGIC_VECTOR (5 downto 0) := "000010";
CONSTANT BEQ : STD_LOGIC_VECTOR (5 downto 0) := "000100";
CONSTANT RTE_opcode : STD_LOGIC_VECTOR (5 downto 0) := "001000";
CONSTANT JAL_opcode : STD_LOGIC_VECTOR (5 downto 0) := "000101";
CONSTANT RET_opcode : STD_LOGIC_VECTOR (5 downto 0) := "000110";
begin
-------------------------------------------------------------------------------------------------------------------------------
-- Kill_IF:
-- da la orden de matar la instrucci�n que se ha le�do en Fetch
-- Se debe activar cada vez que se salte (entrada salto_tomado), ya que por defecto se ha hecho el fetch de la instrucci�n siguiente al salto y si se salta no hay que ejecutarla
-- IMPORTANTE: 
-- 	* si una instrucci�n de salto no tiene sus operandos disponibles no sabe si debe saltar o no (para el BEQ), o a d�nde saltar en el caso de la RET. Da igual lo que diga salto tomado. Hay que parar y esperar a tener los operandos
-- 	* si la instrucci�n que hay en ID no es v�lida hay que ignorarla cuando nos dice que va a saltar (igual que si nos dice cualuier otra cosa), s�lo hacmeos caso a las instrucciones v�lidas
-- Completar: activar Kill_IF cuando proceda
	
	Kill_IF <= '1' when ((salto_tomado = '1') AND (valid_I_ID = '1') AND (BEQ_rs = '0') AND (BEQ_rt = '0') AND (RET_rs = '0')) else '0'; 
-------------------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------------------
-- Detecci�n de dependencias de datos:	
-- Completar:
-- El c�digo incluye un ejemplo: dep_rs_EX. Deb�is completar el resto de opciones.	
	
	-- las I en ID y EX son v�lidas. La I en EX escribe en el registro rs y la I en ID lee rs (NOP, RTE y JAL no leen rs)
	dep_rs_EX 	<= 	'1' when ((valid_I_EX = '1') AND (valid_I_ID = '1') AND (Reg_Rs_ID = RW_EX) and (RegWrite_EX = '1') and (IR_op_code /= NOP) and (IR_op_code /= RTE_opcode) and (IR_op_code /= JAL_opcode))	else '0';
								
	-- la I en Mem escribe en el registro rs y la I en ID no es un NOP ni RTE ni JAL
	dep_rs_Mem	<= 	'1' when ((valid_I_MEM = '1') AND (valid_I_ID = '1') AND (Reg_Rs_ID = RW_Mem) AND (RegWrite_Mem = '1') AND (IR_op_code /= NOP) and (IR_op_code /= RTE_opcode) and (IR_op_code /= JAL_opcode)) else '0';
							
	-- Dependencia de enteros. La I en EX escribe en el registro rt y la I en ID no es un NOP ni un Lw, ni RTE
	dep_rt_EX	<= 	'1' when ((valid_I_EX = '1') AND (valid_I_ID = '1') AND (Reg_Rt_ID = RW_EX) AND (RegWrite_EX = '1') AND (IR_op_code /= NOP) and (IR_op_code /= LW) and (IR_op_code /= RTE_opcode)) else '0';
								
	-- Dependencia de enteros. La I en Mem escribe en el registro rt y la I en ID no es un NOP ni un Lw, ni RTE
	dep_rt_Mem	<= 	'1' when ((valid_I_MEM = '1') AND (valid_I_ID = '1') AND (Reg_Rt_ID = RW_Mem) AND (RegWrite_Mem = '1') AND (IR_op_code /= NOP) and (IR_op_code /= LW) and (IR_op_code /= RTE_opcode)) else '0';

-------------------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------------------
-- Completar:
-- Riesgos de datos:
	-- 1) lw_uso: Si hay dependencia y la instrucci�n en EX es lw tenemos un lw_uso
	ld_uso_rs <= '1' when ((dep_rs_EX = '1') AND (MemRead_EX = '1')) else '0';
	ld_uso_rt <= '1' when ((dep_rt_EX = '1') AND (MemRead_EX = '1')) else  '0';	
									
	-- 2) BEQ: si hay dependencias y la I en ID es un BEQ es un riesgo porque el BEQ lee los datos en ID, y no tenemos red de cortos en esa etapa
	BEQ_rs	<= 	'1' when ((IR_op_code = BEQ) AND ((dep_rs_EX = '1') OR (dep_rs_Mem = '1'))) else '0';
	BEQ_rt	<= 	'1' when ((IR_op_code = BEQ) AND ((dep_rt_EX = '1') OR (dep_rt_Mem = '1'))) else '0';
		
	-- 3) RET: Dependencia similar al BEQ, pero hay que tener en cuenta que RET s�lo usa Rs
	
	RET_rs	<= 	'1' when ((IR_op_code = RET_opcode) AND ((dep_rs_EX = '1') OR (dep_rs_Mem = '1'))) else '0';
	
	-- 4) JAL: se puede gestionar de varias formas. Una de ellas es detener. No es obligatorio detener en los JAL, pero si se hace usad estas se�ales. Si no necesit�is detener, basta con dejarlas a 0
	
	JAL_uso_rs	<= 	'0';
	JAL_uso_rt <= 	'0';	
	
	-- Si se cumple alguna de las condiciones de riesgo de datos se detienen las etapas IF e ID
	riesgo_datos_ID <= BEQ_rt OR BEQ_rs OR ld_uso_rs OR ld_uso_rt OR RET_rs OR JAL_uso_rs OR JAL_uso_rt;
	-- IMPORTANTE: s�lo hay riesgos de datos si la instrucci�n en ID es v�lida
	-- Si se da la orden de parar en MEM, tambi�n hay que parar ID. En el proyecto 1 no hace falta, pero lo ponemos para no tener que tocarlo luego
	Parar_ID <= '1' when ((riesgo_datos_ID = '1')) else '0';
-------------------------------------------------------------------------------------------------------------------------------
	-- parar_MIPS: se utiliza para parar todo el procesador cuando la memoria no puede realizar la operaci�n solicitada en el ciclo actual (es decir cuando Mem_ready es 0). 
	-- �Por qu� paramos todo el procesador y no s�lo la etapa de memoria y las anteriores? 
		-- La raz�n es que si no se detien se pueden perder datos que se iban a anticipar. En el siguiente ejemplo se ve:
		-- ADD R1, R2, R3  F  D  E  M  W
		-- LW   R8, 0(R7)     F  D  E  M  M  M  M  M  W
		-- ADD R6, R1, R4        F  D  E  E  E  E  E  M  W
		-- ADD R6 no puede leer R1 en ID, pero no para porque puede anticiparlo. Sin embargo el LW R8 detiene su ejecuci�n varios ciclos porque la memoria no est� preparada.
		-- Si permitimos que ADD R1 continue, el dato que queriamos anticipar desaparece, y cuando ADD R6 vaya a leerlo no estar�. 
		-- La soluci�n es parar tambi�n ADD R1:
		-- ADD R1, R2, R3  F  D  E  M  W  W  W  W  W  
		-- LW   R8, 0(R7)     F  D  E  M  M  M  M  M  W
		-- ADD R6, R1, R4        F  D  E  E  E  E  E  M  W
		-- De esta forma ADD R6 puede realizar su anticipaci�n. Escribir varias veces el mismo dato no consume energ�a, as� que tampoco hay ninguna penalizaci�n real. En todo caso, si no quer�is escribir varias veces el mismo dato en BR, pod�is inhabilitar la escritura en el banco de registros cuando se detiene el mips.
	-- Completar:	
	parar_MIPS <= '1' when (Mem_ready = '0') else '0';
-------------------------------------------------------------------------------------------------------------------------------
end Behavioral;


