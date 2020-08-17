library IEEE;

use IEEE.std_logic_unsigned.all;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_1164.all;

entity ledc8x8 is
port (
SMCLK: in std_logic; 
RESET: in std_logic;
ROW : out std_logic_vector(0 to 7);
LED: out std_logic_vector(0 to 7)
);
end entity ledc8x8;

architecture behavioral of ledc8x8 is
signal lag : std_logic_vector(21 downto 0);
signal clk1: std_logic;
signal clk2: std_logic;
signal reg: std_logic_vector(7 downto 0);
signal cnt_row: std_logic_vector(7 downto 0);

begin
process(SMCLK, RESET, clk1, reg)
	begin
    if RESET = '1' then
        reg <= (others => '0');
    elsif (SMCLK'event) and (SMCLK = '1') then
        reg <= reg + 1;
    end if;      
    clk1 <= reg(7);
end process;

process(SMCLK, RESET, clk2, lag)
begin
   if RESET = '1' then
      lag <= (others => '0');
   elsif (SMCLK'event) and (SMCLK = '1') then
      lag <= lag + 1;
   end if;
   
   if (clk2 = '0') then
      clk2 <= lag(21);
   end if;
end process;

process(clk1, RESET)
	begin
    if RESET = '1' then
        cnt_row <= "10000000";
    elsif (clk1'event AND clk1 = '1') then
        cnt_row <= cnt_row(0) & cnt_row(7 downto 1);
    end if;
end process;

process(clk2, cnt_row)
    begin
       if (clk2 = '1') then
        case cnt_row is
        when "10000000" =>  LED <= "00111111";
        when "01000000" =>  LED <= "01011111";
        when "00100000" =>  LED <= "01011111";
        when "00010000" =>  LED <= "01010110";
        when "00001000" =>  LED <= "01010101";
        when "00000100" =>  LED <= "00110011";
        when "00000010" =>  LED <= "11110101"; 
        when "00000001" =>  LED <= "11110110";
        when others    =>   LED <= "11111111"; 
    end case;
 else  
        case cnt_row is
        when "10000000" =>  LED <= "00111111";
        when "01000000" =>  LED <= "01011111";
        when "00100000" =>  LED <= "01011111";
        when "00010000" =>  LED <= "01011111";
        when "00001000" =>  LED <= "01011111";
        when "00000100" =>  LED <= "00111111";
        when "00000010" =>  LED <= "11111111"; 
        when "00000001" =>  LED <= "11111111";
        when others    =>   LED <= "11111111"; 
    end case;  
 end if;
end process;

ROW <= cnt_row;     
end behavioral;