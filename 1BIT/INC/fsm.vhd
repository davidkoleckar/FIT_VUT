-- fsm.vhd: Finite State Machine
-- Author:David Koleckar xkolec07 
--
library ieee;
use ieee.std_logic_1164.all;
-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity fsm is
port(
   CLK         : in  std_logic;
   RESET       : in  std_logic;

   -- Input signals
   KEY         : in  std_logic_vector(15 downto 0);
   CNT_OF      : in  std_logic;

   -- Output signals
   FSM_CNT_CE  : out std_logic;
   FSM_MX_MEM  : out std_logic;
   FSM_MX_LCD  : out std_logic;
   FSM_LCD_WR  : out std_logic;
   FSM_LCD_CLR : out std_logic
);
end entity fsm;

-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of fsm is
   type t_state is (TEST, TEST1, TEST2, TEST3, TEST4, TEST5, TEST6, TEST7, TEST8, TEST9, TEST10, TEST11, TEST12, TEST13, TEST14, TEST15, TEST16, TEST17, WRONG, FINISH, PRINT_MESSAGE_OK, PRINT_MESSAGE_WRONG);
   signal present_state, next_state : t_state;

begin
-- -------------------------------------------------------
sync_logic : process(RESET, CLK)
begin
   if (RESET = '1') then
      present_state <= TEST1;
   elsif (CLK'event AND CLK = '1') then
      present_state <= next_state;
   end if;
end process sync_logic;

-- -------------------------------------------------------
next_state_logic : process(present_state, KEY, CNT_OF)
begin
   case (present_state) is
   -- - - - - - - - - - - - - - - - - - - - - - - ////  TEST
   when TEST =>
   -- - - - - - - - - - - - - - - - - - - - - - - ////  TEST 1 (1)
   when TEST1 =>
      next_state <= TEST1;
      if (KEY(1) = '1') then
         next_state <= TEST2;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_WRONG;		 
	  elsif (KEY(15 downto 0) /= "0000000000000000") then
	     next_state <= WRONG;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - - ////  TEST 2 (5)
   when TEST2 =>
      next_state <= TEST2;
      if (KEY(5) = '1') then
         next_state <= TEST3;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_WRONG;		 
	  elsif (KEY(15 downto 0) /= "0000000000000000") then
	     next_state <= WRONG;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - - ////  TEST 3 (6)
   when TEST3 =>
      next_state <= TEST3;
      if (KEY(6) = '1') then
         next_state <= TEST4;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_WRONG;		 
	  elsif (KEY(15 downto 0) /= "0000000000000000") then
	     next_state <= WRONG;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - - ////  TEST 4 (8,0->druhe heslo)
    when TEST4 =>
      next_state <= TEST4;
      if (KEY(8) = '1') then
         next_state <= TEST5;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_WRONG;		 
      elsif (KEY(0) = '1') then
        next_state <= TEST11;	  
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - - ////  TEST 5 (0) 
   when TEST5 =>
      next_state <= TEST5;
      if (KEY(0) = '1') then
         next_state <= TEST6;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_WRONG;		 
	  elsif (KEY(15 downto 0) /= "0000000000000000") then
	     next_state <= WRONG;
      end if;

   -- - - - - - - - - - - - - - - - - - - - - - - ////  TEST 6 (1) 
   when TEST6 =>
      next_state <= TEST6;
      if (KEY(1) = '1') then
         next_state <= TEST7;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_WRONG;		 
	  elsif (KEY(15 downto 0) /= "0000000000000000") then
	     next_state <= WRONG;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - - ////  TEST 7 (5) 
   when TEST7 =>
      next_state <= TEST7;
      if (KEY(5) = '1') then
         next_state <= TEST8;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_WRONG;		 
	  elsif (KEY(15 downto 0) /= "0000000000000000") then
	     next_state <= WRONG;
      end if;
  -- - - - - - - - - - - - - - - - - - - - - - - ////  TEST 8 (7) 
   when TEST8 =>
      next_state <= TEST8;
      if (KEY(7) = '1') then
         next_state <= TEST9;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_WRONG;		 
	  elsif (KEY(15 downto 0) /= "0000000000000000") then
	     next_state <= WRONG;
      end if;	
   -- - - - - - - - - - - - - - - - - - - - - - - ////  TEST 9 (1) 
   when TEST9 =>
      next_state <= TEST9;
      if (KEY(1) = '1') then
         next_state <= TEST10;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_WRONG;		 
	  elsif (KEY(15 downto 0) /= "0000000000000000") then
	     next_state <= WRONG;
      end if;	
   -- - - - - - - - - - - - - - - - - - - - - - - ////  TEST 10 (0) 
   when TEST10 =>
      next_state <= TEST10;
      if (KEY(0) = '1') then
         next_state <= TEST17;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_WRONG;		 
	  elsif (KEY(15 downto 0) /= "0000000000000000") then
	     next_state <= WRONG;
     end if;
        --------------------- DRUHE HESLO -----------------------------
 

   -- - - - - - - - - - - - - - - - - - - - - - - ////  TEST 11 (0) //druhe
   when TEST11 =>
      next_state <= TEST11;
      if (KEY(0) = '1') then
         next_state <= TEST12;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_WRONG;		 
	  elsif (KEY(15 downto 0) /= "0000000000000000") then
	     next_state <= WRONG;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - - ////  TEST 12 (2) //druhe
   when TEST12 =>
      next_state <= TEST12;
      if (KEY(2) = '1') then
         next_state <= TEST13;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_WRONG;		 
	  elsif (KEY(15 downto 0) /= "0000000000000000") then
	     next_state <= WRONG;
      end if;	
   -- - - - - - - - - - - - - - - - - - - - - - - ////  TEST 13 (2) //druhe
   when TEST13 =>
      next_state <= TEST13;
      if (KEY(2) = '1') then
         next_state <= TEST14;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_WRONG;		 
	  elsif (KEY(15 downto 0) /= "0000000000000000") then
	     next_state <= WRONG;
      end if;	
   -- - - - - - - - - - - - - - - - - - - - - - - ////  TEST 14 (4) //druhe
   when TEST14 =>
      next_state <= TEST14;
      if (KEY(4) = '1') then
         next_state <= TEST15;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_WRONG;		 
	  elsif (KEY(15 downto 0) /= "0000000000000000") then
	     next_state <= WRONG;
      end if;		
         -- - - - - - - - - - - - - - - - - - - - - - - ////  TEST 15 (4) //druhe
   when TEST15 =>
      next_state <= TEST15;
      if (KEY(4) = '1') then
         next_state <= TEST16;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_WRONG;		 
	  elsif (KEY(15 downto 0) /= "0000000000000000") then
	     next_state <= WRONG;
      end if;	
         -- - - - - - - - - - - - - - - - - - - - - - - ////  TEST 16 (3) //druhe
   when TEST16 =>
      next_state <= TEST16;
      if (KEY(3) = '1') then
         next_state <= TEST17;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_WRONG;		 
	  elsif (KEY(15 downto 0) /= "0000000000000000") then
	     next_state <= WRONG;
      end if;	
   -- - - - - - - - - - - - - - - - - - - - - - - ////  TEST 17 (#)
   when TEST17 =>
      next_state <= TEST17;
      if (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_OK;	 
	  elsif (KEY(15 downto 0) /= "0000000000000000") then
	     next_state <= WRONG;
      end if;	  
   -- - - - - - - - - - - - - - - - - - - - - - -
   when WRONG =>
      next_state <= WRONG;
      if (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_WRONG;	 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_MESSAGE_OK =>
      next_state <= PRINT_MESSAGE_OK;
      if (CNT_OF = '1') then
		next_state <= FINISH;
	  end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_MESSAGE_WRONG =>
      next_state <= PRINT_MESSAGE_WRONG;
      if (CNT_OF = '1') then
		next_state <= FINISH;
	  end if;
   -- - - - - - - - - - - - - - - - - - - - - - -   
   when FINISH =>
      next_state <= FINISH;
      if (KEY(15) = '1') then
         next_state <= TEST1; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when others =>
      next_state <= TEST;
   end case;
end process next_state_logic;

-- -------------------------------------------------------
output_logic : process(present_state, KEY)
begin
   FSM_CNT_CE     <= '0';
   FSM_MX_MEM     <= '0';
   FSM_MX_LCD     <= '0';
   FSM_LCD_WR     <= '0';
   FSM_LCD_CLR    <= '0';

   case (present_state) is

   -- - - - - - - - - - - - - - - - - - - - - - -   
   when TEST =>
   -- - - - - - - - - - - - - - - - - - - - - - -   
   when WRONG =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;		  
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_MESSAGE_WRONG =>
      FSM_CNT_CE     <= '1';
      FSM_MX_LCD     <= '1';
      FSM_LCD_WR     <= '1';
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_MESSAGE_OK =>
      FSM_CNT_CE     <= '1';
      FSM_MX_LCD     <= '1';
      FSM_LCD_WR     <= '1';
	  FSM_MX_MEM     <= '1';
   -- - - - - - - - - - - - - - - - - - - - - - -   
   when FINISH =>
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when others =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -	  
   end case;
end process output_logic;

end architecture behavioral;


