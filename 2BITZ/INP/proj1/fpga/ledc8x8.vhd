library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.std_logic_unsigned.all;

entity ledc8x8 is
port (
   SMCLK   : in std_logic;
   RESET   : in std_logic;
   ROW_0      : out std_logic;
   ROW_1      : out std_logic;
   ROW_2      : out std_logic;
   ROW_3      : out std_logic;
   ROW_4      : out std_logic;
   ROW_5      : out std_logic;
   ROW_6      : out std_logic;
   ROW_7      : out std_logic;
   COL_0      : out std_logic;
   COL_1      : out std_logic;
   COL_2      : out std_logic;
   COL_3      : out std_logic;
   COL_4      : out std_logic;
   COL_5      : out std_logic;
   COL_6      : out std_logic;
   COL_7      : out std_logic
);
end ledc8x8;

architecture Behavioral of ledc8x8 is
    signal cnt_halfsec               : std_logic_vector(21 downto 0);
	signal cnt_rows                  : std_logic_vector(8 downto 0);
	signal rows                      : std_logic_vector(7 downto 0);
	signal cols                      : std_logic_vector(7 downto 0);
    signal reset_cnt_rows            : std_logic;
	signal half_enable               : std_logic;
	signal half_stop                 : std_logic;
	signal shift_register            : std_logic;

begin

   c_row : process (SMCLK, RESET, reset_cnt_rows)
   begin
   if RESET='1' or reset_cnt_rows='1' then
      cnt_rows <= (others => '0');
   elsif SMCLK='1' and SMCLK'event then
         cnt_rows <= cnt_rows + 1;
   end if;
   end process c_row;

   c_halfsec : process (SMCLK, RESET)
   begin
   if RESET='1' then
      cnt_halfsec <= (others => '0');
   elsif SMCLK='1' and SMCLK'event and half_stop = '0' then
         cnt_halfsec <= cnt_halfsec + 1;
   end if;
   end process c_halfsec;

   compare_row : process(SMCLK, RESET)
   begin
   if RESET='1' then
      shift_register <= '0';
      reset_cnt_rows <= '0';
   elsif (SMCLK'event and SMCLK ='1') then
      if cnt_rows(8) = '1' then
         shift_register <= '1';
	 reset_cnt_rows <= '1';
      else
         shift_register <= '0';
         reset_cnt_rows <= '0';
      end if;
   end if;
   end process compare_row;


   compare_halfsec : process(SMCLK,RESET)
   begin
   if RESET='1' then
      half_enable <= '0';
	  half_stop <= '0';
   elsif (SMCLK'event and SMCLK ='1') then
      if cnt_halfsec = "1111010000100100000000" then
         half_enable <= '1';
		 half_stop <= '1';
      end if;
   end if;
   end process compare_halfsec;

   rotate_rows: process(SMCLK, RESET)
   begin
      if RESET = '1' then
         rows <= (0 => '1', others => '0');
      elsif SMCLK'event and SMCLK = '1' then
         if shift_register = '1' then
            rows <= rows(6 downto 0) & rows(7);
         end if;
      end if;
   end process rotate_rows;
   
   decoder : process(SMCLK, RESET)
   begin
   if RESET = '1' then
      cols <= "11111111";
   elsif (SMCLK'event and SMCLK ='1') then
      if (half_enable = '0') then
         case rows is
            when "00000001" => cols <= "00001111";
            when "00000010" => cols <= "11101111";
            when "00000100" => cols <= "11101111";
            when "00001000" => cols <= "01101111";
            when "00010000" => cols <= "10011111";
            when "00100000" => cols <= "11111111";
            when "01000000" => cols <= "11111111";
            when "10000000" => cols <= "11111111";
            when others => cols <= "00000000";
         end case;
      else
         case rows is
            when "00000001" => cols <= "00001111";
            when "00000010" => cols <= "11101111";
            when "00000100" => cols <= "11101111";
            when "00001000" => cols <= "01101000";
            when "00010000" => cols <= "10011010";
            when "00100000" => cols <= "11111000";
            when "01000000" => cols <= "11111011";
            when "10000000" => cols <= "11111011";
            when others => cols <= "00000000";
         end case;
      end if;
   end if;
   end process decoder;

   ROW_0 <= rows(0);
   ROW_1 <= rows(1);
   ROW_2 <= rows(2);
   ROW_3 <= rows(3);
   ROW_4 <= rows(4);
   ROW_5 <= rows(5);
   ROW_6 <= rows(6);
   ROW_7 <= rows(7);
   COL_0 <= cols(7);
   COL_1 <= cols(6);
   COL_2 <= cols(5);
   COL_3 <= cols(4);
   COL_4 <= cols(3);
   COL_5 <= cols(2);
   COL_6 <= cols(1);
   COL_7 <= cols(0);

end Behavioral;

