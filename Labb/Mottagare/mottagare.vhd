----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    17:04:16 02/03/2012 
-- Design Name: 
-- Module Name:    mottagare - Behavioral 
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
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

---- Uncomment the following library declaration if instantiating
---- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity mottagare is
        port(
        clk: in std_logic;
        rst: in std_logic;
        insignal: in std_logic;
        led: out std_logic;
        seg7: out std_logic_vector(3 downto 0));
end;

architecture behav of mottagare is
        --type fsm is integer;
        signal state : integer;
        signal count : integer;
        signal bitcount : integer;
        signal regis : std_logic_vector(4 downto 0);

begin  
        process(clk,rst)
        begin
                if rst = '1' then
                        state <= 0;
                        count <= 0;
                        bitcount <= 0;
                        regis <= (others=>'0');
                elsif rising_edge(clk) then
                        if state = 0 then
                                if insignal = '1' then
                                        state <= 1;
                                        count <= 0;
                                end if;
                        elsif state = 1 then
                                count <= count + 1;
                                if count = 7 then
                                        if insignal = '1' then
                                                state <= 2;
                                                count <= 0;
                                        else   
                                                state <= 0;
                                                count <= 0;
                                        end if;
                                end if;
                        elsif state = 2 then
                                if count = 15 then
                                        if bitcount = 4 then
                                                state <= 3;
                                                count <= 0;
                                                bitcount <= 0;
                                        else
                                                regis(4 downto 0) <= insignal & regis(4 downto 1);
                                                bitcount <= bitcount + 1;
                                                count <= 0;
                                        end if;
                                else   
                                        count <= count + 1;
                                end if;
                        elsif state = 3 then
                                if count = 15 then
                                        if insignal = '1' then
                                                led <= '1';
                                                state <= 0;
                                                count <= 0;
                                        else   
                                                if (regis(4) xnor ( not ( (regis(0) xnor regis(1)) xnor (regis(2) xnor regis(3))))) = '1' then -- Pratitetstest
                                                        seg7 <= regis(3 downto 0);
                                                        state <= 0;
                                                        count <= 0;
                                                else   
                                                        led <= '1';
                                                        state <= 0;
                                                        count <= 0;
                                                end if;
                                        end if;
                                end if;
                        else   
                                state <= 0;
                        end if;
                end if;
	end process;
end behav;
