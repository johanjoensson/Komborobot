--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:   09:18:04 02/07/2012
-- Design Name:   
-- Module Name:   /home/grulfen/documents/skola/komborobot/Komborobot/Labb/lab//lab_tb.vhd
-- Project Name:  lab
-- Target Device:  
-- Tool versions:  
-- Description:   
-- 
-- VHDL Test Bench Created by ISE for module: lab
-- 
-- Dependencies:
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
--
-- Notes: 
-- This testbench has been automatically generated using types std_logic and
-- std_logic_vector for the ports of the unit under test.  Xilinx recommends
-- that these types always be used for the top-level I/O of a design in order
-- to guarantee that the testbench will bind correctly to the post-implementation 
-- simulation model.
--------------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.numeric_std.ALL;
 
ENTITY lab_tb IS
END lab_tb;
 
ARCHITECTURE behavior OF lab_tb IS 
 
    -- Component Declaration for the Unit Under Test (UUT)
 
    COMPONENT lab
    PORT(
         clock : IN  std_logic;
         reset : IN  std_logic;
         stroben : IN  std_logic;
         knapp : IN  std_logic_vector(3 downto 0);
         lampa : OUT  std_logic;
         utsignal : OUT  std_logic_vector(3 downto 0)
        );
    END COMPONENT;
    

   --Inputs
   signal clock : std_logic := '0';
   signal reset : std_logic := '0';
   signal stroben : std_logic := '0';
   signal knapp : std_logic_vector(3 downto 0) := (others => '0');

 	--Outputs
   signal lampa : std_logic;
   signal utsignal : std_logic_vector(3 downto 0);

   -- Clock period definitions
   constant clock_period : time := 1us;
 
BEGIN
 
	-- Instantiate the Unit Under Test (UUT)
   uut: lab PORT MAP (
          clock => clock,
          reset => reset,
          stroben => stroben,
          knapp => knapp,
          lampa => lampa,
          utsignal => utsignal
        );

   -- Clock process definitions
   clock_process :process
   begin
		clock <= '0';
		wait for clock_period/2;
		clock <= '1';
		wait for clock_period/2;
   end process;
 

   -- Stimulus process
   stim_proc: process
   begin		
		reset <= '1';
      wait for 10 us;
		reset <= '0';
		knapp <= "1010";
		wait for 5 us;
		stroben <= '1';
		wait for 150 us;
		stroben <= '0';
		knapp <= "1100";
		wait for 5 us;
		stroben <= '1';
		wait for 5 us;
		stroben <= '0';
      wait;
   end process;

END;
