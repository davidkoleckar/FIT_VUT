-- cpu.vhd: Simple 8-bit CPU (BrainFuck interpreter)
-- Copyright (C) 2015 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): David Koleckar / xkolec07
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK   : in std_logic;  -- hodinovy signal
   RESET : in std_logic;  -- asynchronni reset procesoru
   EN    : in std_logic;  -- povoleni cinnosti procesoru
 
   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(12 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- mem[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_RDWR  : out std_logic;                    -- cteni (1) / zapis (0)
   DATA_EN    : out std_logic;                    -- povoleni cinnosti
   
   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA <- stav klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna
   IN_REQ    : out std_logic;                     -- pozadavek na vstup data
   
   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- LCD je zaneprazdnen (1), nelze zapisovat
   OUT_WE   : out std_logic                       -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is

signal ptr_data : std_logic_vector(12 downto 0);
signal ptr_inc : std_logic;
signal ptr_dec : std_logic;

signal cnt_data : std_logic_vector(7 downto 0);
signal cnt_inc : std_logic;
signal cnt_dec : std_logic;

signal pc_data : std_logic_vector(12 downto 0);
signal pc_inc : std_logic;
signal pc_dec : std_logic;

type fsm_state is 
(INIT, DECODE, INCPTR, DECPTR,
INCDATA, INCDATA2, DECDATA, DECDATA2, PRINTDATA, 
 READDATA, STOP, OTHERSSTATE);
signal nstate : fsm_state;
signal pstate : fsm_state;

signal sel1 : std_logic_vector(1 downto 0);
signal sel2 : std_logic_vector(1 downto 0);
signal data_rdata_plus : std_logic_vector(7 downto 0);
signal data_rdata_minus : std_logic_vector(7 downto 0);
signal cnt_data_aux : std_logic;
signal data_rdata_aux : std_logic;

begin
-- pc register
pc_p: process(CLK, RESET)
  begin
    if (RESET = '1') then
      pc_data <= (others => '0');
    elsif (CLK'event and CLK = '1') then
      if (pc_inc='1') then
        pc_data <= pc_data + 1;
      elsif (pc_dec='1') then
        pc_data <= pc_data - 1;
      end if;
	  sel1 <= "10";
    end if;
  end process;
  
-- ptr register
ptr: process(CLK, RESET)
  begin
    if (RESET = '1') then
      ptr_data <= (others => '0');
    elsif (CLK'event and CLK = '1') then
      if (ptr_inc='1') then
        ptr_data <= ptr_data + 1;
      elsif (ptr_dec='1') then
        ptr_data <= ptr_data - 1;
      end if;
	  sel1 <= "01";
    end if;
  end process;
  
-- cnt register
cnt: process(CLK, RESET)
  begin
    if (RESET = '1') then
      cnt_data <= (others => '0');
    elsif (CLK'event and CLK = '1') then
      if (cnt_inc='1') then
        cnt_data <= cnt_data + 1;
      elsif (cnt_dec='1') then
        cnt_data <= cnt_data - 1;
      end if;
    end if;
  end process;
 

--mux1
with sel1 select
	DATA_ADDR <= ptr_data when "01",
				pc_data when "10",
				(others => '0') when others;
  

data_rdata_plus <= DATA_RDATA+"00000001";
data_rdata_minus <= DATA_RDATA-"00000001"; 
--mux2
with sel2 select
	DATA_WDATA <= IN_DATA when "00",
					data_rdata_plus when "01",
					data_rdata_minus when "10",
					(others => '0') when others;

cnt_data_aux <= '1' when (cnt_data = "00000000") else '0';
data_rdata_aux <= '1' when (DATA_RDATA = "00000000") else '0';
  
-- FSM
fsm_pstate: process(CLK, RESET)
  begin
    if (RESET = '1') then
      pstate <= INIT;
    elsif (CLK'event and CLK = '1') then
      if(EN = '1') then
        pstate <= nstate;
      end if;
    end if;
  end process;

fsm_nstate: process (pstate, cnt_data_aux, data_rdata_aux, CLK, RESET, EN, DATA_RDATA, IN_DATA, IN_VLD, OUT_BUSY)
  begin                                                                                                                               
    nstate <= INIT;
    ptr_inc <= '0';
    ptr_dec <= '0';
    pc_inc <= '0';
    pc_dec <= '0';
    cnt_inc <= '0';
    cnt_dec <= '0';
    DATA_EN <= '0';--mozna 1
    OUT_WE <= '0';
    IN_REQ <= '0';
    sel2 <= "00";

    case pstate is
      when INIT =>
        DATA_EN <= '1';
        nstate <= DECODE;
    
      when DECODE =>
        case DATA_RDATA is
          when X"3E" =>
            nstate <= INCPTR;
          when X"3C" =>
            nstate <= DECPTR;
          when X"2B" =>
            nstate <= INCDATA;
          when X"2D" =>
            nstate <= DECDATA;
          when X"2E" =>
            nstate <= PRINTDATA;
          when X"2C" =>
            nstate <= READDATA;
          when X"00" =>
            nstate <= STOP;
          when others =>
            nstate <= OTHERSSTATE;
        end case;
		
	      when INCPTR =>
        ptr_inc <= '1';
        pc_inc  <= '1';
        nstate  <=INIT;
      
      when DECPTR =>
        ptr_dec <= '1';
        pc_inc  <= '1';
        nstate  <= INIT;
    
      when INCDATA =>
        DATA_RDWR <= '0';
        DATA_EN   <= '1';
        nstate    <= INCDATA2;
    
      when INCDATA2 =>
        sel2         <= "01";
        DATA_RDWR <= '1';
        DATA_EN   <= '1';
        pc_inc      <= '1';
        nstate      <= INIT;
      
      when DECDATA =>
        DATA_RDWR <= '0';
        DATA_EN   <= '1';
        nstate      <= DECDATA2;
      
      when DECDATA2 =>
        sel2         <= "10";
        DATA_RDWR <= '1';
        DATA_EN   <= '1';
        pc_inc      <= '1';
        nstate      <= INIT; 
    
      when PRINTDATA =>
        if(OUT_BUSY = '1') then
          nstate <= PRINTDATA;
        else
          DATA_RDWR <= '0';
          OUT_DATA <= DATA_RDATA; 
          OUT_WE <= '1';
          pc_inc <= '1';
          nstate <= INIT;
        end if;
  
      when READDATA =>
        IN_REQ <= '1';
        if (IN_VLD = '1') then
          sel2         <= "00";
          DATA_RDWR <= '1';
          DATA_EN <= '1';
          pc_inc <= '1';
          nstate <= INIT;  
        else
          nstate <= READDATA;
        end if;
  
      when OTHERSSTATE =>
        pc_inc <= '1';
        nstate <= INIT;
      
      when STOP =>
        nstate <= STOP;
    
      when others => null;

    end case;
  end process;
end behavioral;
 
