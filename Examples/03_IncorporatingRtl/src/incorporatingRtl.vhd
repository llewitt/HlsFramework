library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity incorporatingRtl is
  Port (clk : in std_logic;
        rst : in std_logic;
        saxis_tready : out std_logic;
        saxis_tvalid : in std_logic;
        saxis_tlast : in std_logic;
        saxis_tdata : in std_logic_vector(31 downto 0);
        maxis_tready : in std_logic;
        maxis_tvalid : out std_logic;
        maxis_tlast : out std_logic;
        maxis_tdata : out std_logic_vector(31 downto 0));
end incorporatingRtl;

architecture Behavioral of incorporatingRtl is
begin
  process(clk)
    variable square : signed((2 * saxis_tdata'length) - 1 downto 0);
  begin
    if clk'event AND clk = '1' then
      square := signed(saxis_tdata) * signed(saxis_tdata);
        
      if signed(saxis_tdata) < 0 then
        maxis_tdata <= saxis_tdata;
      else
        maxis_tdata <= std_logic_vector(square (saxis_tdata'length - 1 downto 0));
      end if;

      saxis_tready <= maxis_tready;
      maxis_tvalid <= saxis_tvalid;
      maxis_tlast <= saxis_tlast;
    end if;
  end process;
end Behavioral;

