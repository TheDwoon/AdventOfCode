create table aoc_input_all (
    YEAR NUMBER,
    DAY NUMBER,
    VERSION NUMBER,
    LINE NUMBER,
    CONTENT VARCHAR2(4000)
)
/
create unique index aoc_input_all_u1 on dwieland.aoc_input_all(YEAR, DAY, VERSION, LINE);
/