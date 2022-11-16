create table aoc_input (
    YEAR NUMBER,
    DAY NUMBER,
    LINE NUMBER,
    CONTENT VARCHAR2(4000)
)
/
create unique index aoc_input_u1 on dwieland.aoc_input(YEAR, DAY, LINE);
/