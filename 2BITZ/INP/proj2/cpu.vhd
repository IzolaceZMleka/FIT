-- cpu.vhd: Simple 8-bit CPU (BrainFuck interpreter)
-- Copyright (C) 2015 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): Jan Pavlica - xpavli78
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

 signal cnt_inc : std_logic;
 signal cnt_dec : std_logic;
 signal cnt_counter : std_logic_vector(7 downto 0);
 
 signal pc_inc : std_logic;
 signal pc_dec : std_logic;
 signal pc_out : std_logic_vector(12 downto 0);
 signal pc_counter : std_logic_vector(11 downto 0);

 signal ptr_inc : std_logic;
 signal ptr_dec : std_logic;
 signal ptr_out : std_logic_vector(12 downto 0);
 signal ptr_counter : std_logic_vector(11 downto 0);
 
 signal tmp : std_logic_vector(7 downto 0);
 signal ld_s : std_logic;
 
 signal sel_mx4 : std_logic_vector(1 downto 0);
 signal sel_mx2 : std_logic;
 
 signal inc_data : std_logic_vector(7 downto 0);
 signal dec_data : std_logic_vector(7 downto 0);

 signal rdata_zero : std_logic;
 signal cnt_zero : std_logic;
 
 type inst_type is (inc_ptr, dec_ptr, inc_val, dec_val, while_b, while_e, print, get, tmp_in, tmp_out, stop, other);
 signal dec : inst_type;
 
 type fsm_state is(sidle, sfetch0, sfetch1, sdecode, sinc_ptr, sdec_ptr, sinc_val, sdec_val, swhile_b, swhile_e, sprint,
						 sget, sget2, stmp_in, stmp_out, sstop, sother,
						 sinc_val2, sdec_val2, sinc_val3, sdec_val3, sprint2, sprint3, sprint4,
						 stmp_out2, stmp_out3, stmp_in2, swhile_b2, swhile_b3, swhile_b4, swhile_b5, swhile_b6,
						 swhile_b10, swhile_e2, swhile_e3, swhile_e4, swhile_e5, swhile_e6, swhile_e10);
 
 signal pstate : fsm_state;
 signal nstate : fsm_state;

begin

-- CNT
process(RESET, CLK)
begin
	if (RESET='1') then
		cnt_counter <= "00000000";  
	elsif (CLK'event) and (CLK='1') then
		if (cnt_dec = '1') then
			cnt_counter <= cnt_counter - '1';
		elsif (cnt_inc = '1') then
			cnt_counter <= cnt_counter + '1';
		end if;
	end if;
end process;

-- PC
process(RESET, CLK)
begin
   if (RESET='1') then
      pc_counter <= "000000000000";  
   elsif (CLK'event) and (CLK='1') then
		if (pc_dec = '1') then
			pc_counter <= pc_counter - '1';
		elsif (pc_inc = '1') then
			pc_counter <= pc_counter + '1';
		end if;
	end if;
end process;

pc_out <= '0'&pc_counter;

-- PTR
process(RESET, CLK)
begin
	if (RESET='1') then
		ptr_counter <= "000000000000";  
	elsif (CLK'event) and (CLK='1') then
		if (ptr_dec = '1') then
			ptr_counter <= ptr_counter - '1';
		elsif (ptr_inc = '1') then
			ptr_counter <= ptr_counter + '1';
		end if;
	end if;
end process;

ptr_out <= '1'&ptr_counter;

-- TMP
process(RESET, CLK)
begin
	if (RESET='1') then
		tmp <= "00000000";
	elsif (CLK'event) and (CLK='1') then
		if (ld_s = '1') then
			tmp <= DATA_RDATA;
		end if;
	end if;
end process;

-- INC DATA
inc_data <= DATA_RDATA + '1';

-- DEC DATA
dec_data <= DATA_RDATA - '1';

-- MX2
process (sel_mx2, pc_out, ptr_out)
begin
	case sel_mx2 is
		when '0' => DATA_ADDR <= pc_out;
		when '1' => DATA_ADDR <= ptr_out;
		when others => DATA_ADDR <= "0000000000000";
	end case;
end process;


-- MX4

process (sel_mx4, IN_DATA, tmp, dec_data, inc_data)
begin
	case sel_mx4 is
		when "00" => DATA_WDATA <= IN_DATA;
		when "01" => DATA_WDATA <= tmp;
		when "10" => DATA_WDATA <= dec_data;
		when "11" => DATA_WDATA <= inc_data;
		when others => DATA_WDATA <= "00000000";
	end case;
end process;

rdata_zero <= '1' when DATA_RDATA = "00000000" else '0';
cnt_zero <= '1' when cnt_counter = "00000000" else '0';


-- dekoder

process (DATA_RDATA)
begin
	case DATA_RDATA is
		when X"3E" => dec <= inc_ptr;
		when X"3C" => dec <= dec_ptr;
		when X"2B" => dec <= inc_val;
		when X"2D" => dec <= dec_val;
		when X"5B" => dec <= while_b;
		when X"5D" => dec <= while_e;
		when X"2E" => dec <= print;
		when X"2C" => dec <= get;
		when X"24" => dec <= tmp_in;
		when X"21" => dec <= tmp_out;
		when X"00" => dec <= stop;
		when others => dec <= other;
	end case;
end process;

-- present state

fsm_pstate: process (RESET, CLK)
begin
	if (RESET='1') then
		pstate <= sidle;
	elsif (CLK'event) and (CLK='1') then
		if(EN = '1') then
			pstate <= nstate;
		end if;
	end if;
end process;

-- FSM

fsm: process (pstate, IN_VLD, OUT_BUSY, dec, DATA_RDATA, rdata_zero, cnt_zero)
begin
-- INIT
	pc_inc <= '0';
	pc_dec <= '0';
	
	ptr_inc <= '0';
	ptr_dec <= '0';
	
	ld_s <= '0';
	
	cnt_inc <= '0';
	cnt_dec <= '0';
	
	DATA_RDWR <= '1';
	DATA_EN <= '0';
	IN_REQ <= '0';
	OUT_WE <= '0';
	-- OUT_DATA <= (others => '0');
	
	sel_mx4 <= "00";
	sel_mx2 <= '0';
	
	
	case pstate is
	-- IDLE
		when sidle => 
			nstate <= sfetch0;
			
		when sfetch0 => 
			nstate <= sfetch1;
			DATA_EN <= '1';
		
		when sfetch1 =>
			nstate <= sdecode;
			
		when sdecode =>
			case dec is
				when inc_ptr =>
					nstate <= sinc_ptr;
				when dec_ptr =>
					nstate <= sdec_ptr;
				when inc_val =>
					nstate <= sinc_val;
					sel_mx2 <= '1';
				when dec_val =>
					nstate <= sdec_val;
					sel_mx2 <= '1';
				when while_b =>
					nstate <= swhile_b;
				when while_e =>
					nstate <= swhile_e;
				when print =>
					nstate <= sprint;
					sel_mx2 <= '1'; -- ADRESA NA PTR
				when get =>
					nstate <= sget;
					sel_mx2 <= '1';
				when tmp_in =>
					nstate <= stmp_in;
					sel_mx2 <= '1';
				when tmp_out =>
					nstate <= stmp_out;
					sel_mx2 <= '1';
					sel_mx4 <= "01";
				when stop =>
					nstate <= sstop;
				when other =>
					nstate <= sother;
				when others => nstate <= sinc_val;
			end case;
		
			-- >
			when sinc_ptr =>
				nstate <= sfetch0;
				ptr_inc <= '1';
				pc_inc <= '1';
			
			-- <
			when sdec_ptr =>
				nstate <= sfetch0;
				ptr_dec <= '1';
				pc_inc <= '1';
				
			-- +
			when sinc_val=>
				nstate <= sinc_val2;
				DATA_EN<='1';
				DATA_RDWR<='1';
				sel_mx2 <= '1';
				sel_mx4 <= "11";
				
			-- + 2
			when sinc_val2 =>
				nstate <= sinc_val3;
				DATA_EN<='1';
				DATA_RDWR<='1';
				sel_mx2 <= '1';
				sel_mx4 <= "11";
				
			-- + 3
			when sinc_val3 =>
				nstate <= sfetch0;
				DATA_EN<='1';
				DATA_RDWR<='0';
				pc_inc <= '1';
				sel_mx2 <= '1';				
				sel_mx4 <= "11";
				
				
			-- -
			when sdec_val=>
				nstate <= sdec_val2;
				DATA_EN<='1';
				DATA_RDWR<='1';
				sel_mx2 <= '1';
				sel_mx4 <= "10";
				
			-- - 2
			when sdec_val2 =>
				nstate <= sdec_val3;
				DATA_EN<='1';
				DATA_RDWR<='1';
				sel_mx2 <= '1';
				sel_mx4 <= "10";
				
			-- - 3
			when sdec_val3 =>
				nstate <= sfetch0;
				DATA_EN<='1';
				DATA_RDWR<='0';
				pc_inc <= '1';
				sel_mx2 <= '1';				
				sel_mx4 <= "10";
				
			-- . 
			when sprint => -- nacitam do RDATA
				sel_mx2 <= '1';
				DATA_EN<='1';
				DATA_RDWR<='1';
				nstate <= sprint2;
				
			-- . 2
			when sprint2 => -- v RDATA potrebna data
				sel_mx2 <= '1';
				DATA_EN<='1';
				nstate <= sprint3;			
				
				
			-- . 2
			when sprint3 =>
				DATA_EN<='1';
				DATA_RDWR<='1';
				OUT_DATA <= DATA_RDATA;
				nstate <= sprint4;
				
			when sprint4 =>
            nstate <= sprint4;
				if(OUT_BUSY = '0') then	
					OUT_WE <= '1';
					nstate <= sfetch0;
					pc_inc <= '1';
				end if;
				
			-- , 	
			when sget =>
				IN_REQ <= '1';
				sel_mx2 <= '1';
				if(IN_VLD = '1') then
					nstate <= sget2;
				else
					nstate <= sget;
				end if;
				
			-- , 2	
			when sget2 =>
			     nstate <= sfetch0;
				  sel_mx2 <= '1';
				  DATA_EN <= '1';
				  DATA_RDWR <= '0';
				  sel_mx4 <= "00";
				  pc_inc <= '1';
				  
			-- $
			when stmp_in =>
				sel_mx2 <= '1';
				nstate <= stmp_in2;
				DATA_EN <= '1';
				DATA_RDWR <= '1';
				ld_s <= '1';
				
			-- $ 2	
			when stmp_in2 =>
				sel_mx2 <= '1';
				nstate <= sfetch0;
				DATA_EN <= '1';
				DATA_RDWR <= '1';
				ld_s <= '1';
				pc_inc <= '1';
				
			-- !
			when stmp_out =>
				nstate <= stmp_out2;
				sel_mx4 <= "01";
				sel_mx2 <= '1';
				DATA_EN <= '1';
				DATA_RDWR <= '0';
	
			-- ! 2
			when stmp_out2 =>
				sel_mx4 <= "01";
				sel_mx2 <= '1';
				nstate <= stmp_out3;
				DATA_EN <= '1';
				DATA_RDWR <= '0';
				
			-- ! 3	
			when stmp_out3 =>
				sel_mx4 <= "01";
				sel_mx2 <= '1';
				nstate <= sfetch0;
				pc_inc <= '1';
				DATA_EN <= '1';
				DATA_RDWR <= '0';
				
			-- NULL
			when sstop =>
				nstate <= sfetch0;
				
			-- other	
			when sother =>
				pc_inc <= '1';
				nstate <= sfetch0;
			
			
			-- [ -- zpristupneni PTR
			
			when swhile_b =>
				nstate <= swhile_b2;
				pc_inc <= '1';
				sel_mx2 <= '1';
				DATA_EN <= '1';
				DATA_RDWR <= '1';
				
			-- [ 2 -- ziskani hodnoty PTR
			when swhile_b2 =>
				nstate <= swhile_b3;
				sel_mx2 <= '1';
				DATA_EN <= '1';
				DATA_RDWR <= '1';
				
			-- [ 3 -- kontrola PTR == 0;
			when swhile_b3 =>
				if(rdata_zero = '1') then -- preskakuji intrukce
					nstate <= swhile_b4;
					cnt_inc <= '1';
				else
					nstate <= sfetch0; -- vykonavam instrukce
				end if;
				
			-- [ 4 -- CNT != 0	
			when swhile_b4 =>
				if(cnt_zero = '1') then -- vyskakuji z cyklu
					nstate <= sfetch0;
				else
					nstate <= swhile_b10; -- pocitam vnorene while
				end if;
				
			when swhile_b10 =>
				nstate <= swhile_b5;
				
			-- [ 5 -- kontroluji dalsi [ ]
			when swhile_b5 =>
				if(dec = while_b) then
					cnt_inc <= '1';
				elsif(dec = while_e) then
					cnt_dec <= '1';
				end if;
				pc_inc <= '1';
				nstate <= swhile_b6;
				
			-- [ 6 -- nacitam dalsi instrukci
			when swhile_b6 =>
				DATA_EN <= '1';
				DATA_RDWR <= '1';
				nstate <= swhile_b4;
				
				
			-- ] -- zpristupneni PTR
			when swhile_e =>
				nstate <= swhile_e2;
				sel_mx2 <= '1';
				DATA_EN <= '1';
				DATA_RDWR <= '1';
				
			-- ] 2 -- ziskani hodnoty PTR
			when swhile_e2 =>
				nstate <= swhile_e3;
				sel_mx2 <= '1';
				DATA_EN <= '1';
				DATA_RDWR <= '1';
				
			-- ] 3 -- kontrola PTR == 0;
			when swhile_e3 =>
				if(rdata_zero = '1') then -- preskakuji intrukce
					nstate <= sfetch0;	
					pc_inc <= '1';
				else
					nstate <= swhile_e4; -- vykonavam instrukce
					cnt_inc <= '1';
					pc_dec <= '1';
				end if;
				
			-- ] 4 -- CNT != 0	
			when swhile_e4 =>
				if(cnt_zero = '1') then -- vyskakuji z cyklu
					nstate <= sfetch0;
				else
					nstate <= swhile_e10; -- pocitam vnorene while
				end if;
				
			when swhile_e10 =>
				nstate <= swhile_e5;	
				
			-- ] 5 -- kontroluji dalsi [ ]
			when swhile_e5 =>
				if(dec = while_b) then
					cnt_dec <= '1';
				elsif(dec = while_e) then
					cnt_inc <= '1';
				end if;
				nstate <= swhile_e6;
				
			-- ] 6 -- nacitam dalsi instrukci
			when swhile_e6 =>
				if(cnt_zero = '1') then
					pc_inc <= '1';
				else
					pc_dec <= '1';
				end if;
				DATA_EN <= '1';
				DATA_RDWR <= '1';
				nstate <= swhile_e4;
	 
		when others => nstate <= sstop;
	end case;

end process;

end behavioral;
