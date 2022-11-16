create or replace package body aoc as
    --
    cursor cs_input(cp_year in number, cp_day in number) is
        select year, day, line, content
        from aoc_input
        order by line;
    --
    cursor cs_input_line(cp_year in number, cp_day in number, cp_line in number) is
        select year, day, line, content
        from aoc_input;
    --
    --
    procedure load_input(
        p_year IN NUMBER,
        p_day IN NUMBER,
        p_line IN NUMBER,
        p_content IN VARCHAR2
    ) is
        --
        r_input cs_input_line%rowtype;
        --
    begin
        --
        open cs_input_line(p_year, p_day, p_line);
        fetch cs_input_line into r_input;
        close cs_input_line;
        --
        if r_input.YEAR is null then
            --
            insert into aoc_input (YEAR, DAY, LINE, CONTENT)
            values (p_year, p_day, p_line, p_content);
            --
        end if;
        --
    end load_input;
    --
    function get_input(
        p_year IN NUMBER,
        p_day IN NUMBER
    ) RETURN input_t IS
        --
        t_input input_t;
        --
    begin
        --
        open cs_input(p_year, p_day);
        fetch cs_input bulk collect into t_input;
        close cs_input;
        --
        return t_input;
        --
    end get_input;
    --
end;