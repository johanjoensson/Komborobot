library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;

entity sender is
  port(
    x : std_logic_vector(3 downto 0);
    clk, strobe : in std_logic;
    u : out std_logic
    );
end sender;


-- ############################################################################
-- G� genom 6 l�gen, synkroniserat via klockan.
-- 0, inv�nta h�g strob (data ligger redo), g� till 1.
-- 1, skicka 1 i 16 klockintervall, g� till 2.
-- 2, skicka X1 i 16 klockintervall, g� till 3.
-- 3, skicka X2 i 16 klockintervall, g� till 4.
-- 4, skicka X4 i 16 klockintervall, g� till 5.
-- 5, skicka X8 i 16 klockintervall, g� till 6.
-- 6, skicka paritetsbit i 16 klockintervall, g� till 7.
-- 7, skicka 0 tills stroben blir l�g, g� till 0.
-- Rst - resetsignal, g� till l�ge 0. Asynkron.
-- ############################################################################
architecture send of sender is
  signal cnt : std_logic_vector (4 downto 0);
  signal state : integer range 0 to 7;
  signal rst : std_logic;
begin
  process (clk, rst)
  begin
    if rst = '1' then
      state <= 0;
    
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
          if strobe /= '0' then
            u <= '0';
          else
            state <= 0;
          end if;
        end case;
    end if;
  end process;   
end send;