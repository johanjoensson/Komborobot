library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;

entity lab is 
        port (
                clock : in std_logic;
		reset : in std_logic;
		stroben : in std_logic;
                knapp : in std_logic_vector(3 downto 0);
                lampa : out std_logic;
                utsignal : out std_logic_vector(3 downto 0));
end lab;

architecture struktur of lab is
        signal u_int : std_logic;
        component mottagare is
                port(
                clk : in std_logic;
                rst : in std_logic;
                insignal : in std_logic;
                led : out std_logic;
                seg7 : out std_logic_vector(3 downto 0));
        end component mottagare;
        component sandare is
                port(
                x : in std_logic_vector(3 downto 0);
                clk, strobe, rst : in std_logic;
                u : out std_logic);
        end component sandare;
begin
        s: sandare
                port map (knapp,clock,stroben,reset,u_int);
        m: mottagare
                port map (clock,reset,u_int,lampa,utsignal);
end struktur;
