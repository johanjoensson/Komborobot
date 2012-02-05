library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity sender is
  port(
    x : std_logic_vector(3 downto 0);
    clk, strobe, rst : in std_logic;
    u : out std_logic
    );
end sender;


-- ############################################################################
-- G�genom 6 l�en, synkroniserat via klockan.
-- 0, inv�ta h� strob (data ligger redo), g�till 1.
-- 1, skicka 1 i 16 klockintervall, g�till 2.
-- 2, skicka X1 i 16 klockintervall, g�till 3.
-- 3, skicka X2 i 16 klockintervall, g�till 4.
-- 4, skicka X4 i 16 klockintervall, g�till 5.
-- 5, skicka X8 i 16 klockintervall, g�till 6.
-- 6, skicka paritetsbit i 16 klockintervall, g�till 7.
-- 7, skicka 0 tills stroben blir l�, g�till 0.
-- Rst - resetsignal, g�till l�e 0. Asynkron.
-- ############################################################################

architecture send of sender is
  signal cnt : std_logic_vector (4 downto 0);
  signal state : integer range 0 to 7;
begin
  process (clk, rst)
  begin
    if rst = '1' then
      state <= 0;
      u <= '0';
    
    elsif rising_edge(clk) then 
      case state is
        when 0 =>
          if strobe = '1' then
            state <= 1;
           cnt <= "00000";
          end if;
        when 1 =>
          if cnt < "10000" then
            cnt <= cnt + '1';
            u <= '1';
          else
            state <= 2;
            cnt <= "00000";
          end if;
        when 2 =>
          if cnt < "10000" then
            cnt <= cnt + '1';
            u <= x(0);
          else
            state <= 3;
            cnt <= "00000";
          end if;
          
          when 3 =>
          if cnt < "10000" then
            cnt <= cnt + '1';
            u <= x(1);
          else
            state <= 4;
            cnt <= "00000";
          end if;
          when 4 =>
          if cnt < "10000" then
            cnt <= cnt + '1';
            u <= x(2);
          else
            state <= 5;
            cnt <= "00000";
          end if;
          
          when 5 =>
          if cnt < "10000" then
            cnt <= cnt + '1';
            u <= x(3);
          else
            state <= 6;
            cnt <= "00000";
          end if;
          when 6 =>
	  if cnt < "10000" then
	    cnt <= cnt + '1';
	    u <= not ((x(0) xnor x(1)) xnor (x(2) xnor x(3)));
    	  else
	    state <= 7;
	    cnt <= "00000";
    	  end if;
	  when 7 =>
          if cnt < "10000" then
                  cnt <= cnt + '1';
                  u <= '0';
          elsif strobe = '1' then
            u <= '0';
          else
            state <= 0;
          end if;
        end case;
    end if;
  end process;   
end send;

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
								seg7 <= (others=>'0');
								led <= '0';
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
                                led <= '0';
                                if count = 15 then
                                        if bitcount = 4 then
                                                regis(3 downto 0) <= regis(4 downto 1);
                                                regis(4) <= insignal;
                                                state <= 3;
                                                count <= 0;
                                                bitcount <= 0;
                                        else
                                                regis(3 downto 0) <= regis(4 downto 1);
                                                regis(4) <= insignal;
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
                                                if (regis(4) xnor ( not ( (regis(0) xnor regis(1)) xnor (regis(2) xnor regis(3))))) = '1' then -- Paritetstest
                                                        seg7 <= regis(3 downto 0);
                                                        state <= 0;
                                                        count <= 0;
                                                else   
                                                        led <= '1';
                                                        state <= 0;
                                                        count <= 0;
                                                end if;
                                        end if;
                                else
                                        count <= count + 1;
                                end if;
                        else   
                                state <= 0;
                        end if;
                end if;
	end process;
end behav;
