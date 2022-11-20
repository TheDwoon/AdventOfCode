create global temporary table aoc_input (
    LINE NUMBER,
    CONTENT VARCHAR2(4000)
)
on commit delete rows
/
create unique index aoc_input_u1 on dwieland.aoc_input(LINE);
/