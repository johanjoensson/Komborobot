library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;

entity sender is
  port(
    x : std_logic_vector(3 downto 0);
    clk, strobe, rst : in std_logic;
    u : out std_logic
    );
end sender;


-- ############################################################################
-- Gå igenom 7 lägen, synkroniserat via klockan.
-- 0, invänta hög strob (data ligger redo), gå till 1.
-- 1, skicka 1 i 16 klockintervall, gå till 2.
-- 2, skicka X1 i 16 klockintervall, gå till 3.
-- 3, skicka X2 i 16 klockintervall, gå till 4.
-- 4, skicka X4 i 16 klockintervall, gå till 5.
-- 5, skicka X8 i 16 klockintervall, gå till 6.
-- 6, skicka paritetsbit i 16 klockintervall, gå till 7.
-- 7, skicka 0 i 16 klockintervall och sedan tills stroben blir låg, gå till 0.
-- Rst - resetsignal, gå till läge 0. Asynkron.
-- ############################################################################
architecture send of sender is
  signal cnt : std_logic_vector (4 downto 0);
  signal state : integer range 0 to 7;
  signal reg : std_logic_vector (3 downto 0);
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
	    reg(3 downto 0) <= x(3 downto 0);
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
            u <= reg(0);
          else
            state <= 3;
            cnt <= "00000";
          end if;
          
          when 3 =>
          if cnt < "10000" then
            cnt <= cnt + '1';
            u <= reg(1);
          else
            state <= 4;
            cnt <= "00000";
          end if;
          when 4 =>
          if cnt < "10000" then
            cnt <= cnt + '1';
            u <= reg(2);
          else
            state <= 5;
            cnt <= "00000";
          end if;
          
          when 5 =>
          if cnt < "10000" then
            cnt <= cnt + '1';
            u <= reg(3);
          else
            state <= 6;
            cnt <= "00000";
          end if;
          when 6 =>
	  if cnt < "10000" then
	    cnt <= cnt + '1';
	    u <= not ((reg(0) xnor reg(1)) xnor (reg(2) xnor reg(3)));
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
