entity lab is 
        port (
                clk,rst,strobe : in std_logic;
                x : in std_logic_vector(3 downto 0);
                led : out std_logic;
                seg7 : out std_logic_vector;
end lab;

architecture struktur of lab is
        signal u_int : std_logic;
        component mottagare
                port(
                clk: in std_logic;
                rst: in std_logic;
                insignal: in std_logic;
                led: out std_logic;
                seg7: out std_logic_vector(3 downto 0));
        end component;
        component sandare
                port(
                x : std_logic_vector(3 downto 0);
                clk, strobe, rst : in std_logic;
                u : out std_logic);
        end component;
begin
        m: mottagare
                port map (clk=>clk,rst=>rst,u_int=>insignal,led=>led,seg7=>seg7);
        s: sandare
                port map (x=>x,clk=>clk,strobe=>strobe,rst=>rst,u_int=>u);
end struktur;
