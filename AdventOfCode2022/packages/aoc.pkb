create or replace package body aoc as
    --
    g_package varchar2(64) := 'aoc';
    --
    g_batch_id number := null;
    g_year number := null;
    g_day number := null;
    g_version number := null;
    --
    cursor cs_input(cp_year in number, cp_day in number, cp_version in number) is
        select year, day, line, content
        from aoc_input
        where year = cp_year
          and day = cp_day
          and version = cp_version
        order by line;
    --
    cursor cs_input_line(cp_year in number, cp_day in number, cp_version in number, cp_line in number) is
        select year, day, line, content
        from aoc_input
        where year = cp_year
          and day = cp_day
          and version = cp_version
          and line = cp_line;
    --
    --
    procedure init(
        p_year in number,
        p_day in number,
        p_version in number
    ) is
        --
        --
    begin
        --
        g_batch_id := aoc_debug_s.nextval;
        --
        g_year := p_year;
        g_day := p_day;
        --
        if p_version is null then
            g_version := 0;
        else
            g_version := p_version;
        end if;
        --
        debug(g_package, 'init', g_year || ': day ' || g_day || ' @' || g_version);
        --
    end init;
    --
    procedure debug(
        p_package in varchar2,
        p_method in varchar2,
        p_message in varchar2,
        p_level in number default 10
    ) is
        --
        pragma autonomous_transaction;
        --
    begin
        --
        if p_message is not null then
            --
            dbms_output.PUT_LINE('[' || g_batch_id || '] ' || p_package || '.' || p_method || ': ' || p_message);
            --
            insert into aoc_debug (LOG_ID, BATCH_ID, PACKAGE, METHOD, MESSAGE, LOG_LEVEL)
            values (aoc_debug_s.nextval, g_batch_id, p_package, p_method, p_message, p_level);
            --
            COMMIT;
            --
        end if;
        --
    end debug;
    --
    procedure load_input(
        p_line in number,
        p_content in varchar2
    ) is
        --
        --
    begin
        --
        load_input(g_year, g_day, g_version, p_line, p_content);
        --
    end load_input;
    --
    procedure load_input(
        p_year in number,
        p_day in number,
        p_version in number,
        p_line in number,
        p_content in varchar2
    ) is
        --
        r_input           cs_input_line%rowtype;
        l_method constant varchar2(32) := 'load_input';
        --
    begin
        --
        open cs_input_line(p_year, p_day, p_version, p_line);
        fetch cs_input_line into r_input;
        close cs_input_line;
        --
        if r_input.year is null then
            --
            insert into aoc_input (year, day, version, line, content)
            values (p_year, p_day, p_version, p_line, p_content);
            --
            debug(g_package, l_method, 'inserted line: '
                || 'p_year: ' || p_year
                || ', p_day: ' || p_day
                || ', p_version: ' || p_version
                || ', p_line: ' || p_line
                || ', p_content: ' || p_content
                );
            --
        else
            --
            update aoc_input
            set content = p_content
            where year = p_year
              and day = p_day
              and version = p_version
              and line = p_line;
            --
            debug(g_package, l_method, 'updated line: '
                || 'p_year: ' || p_year
                || ', p_day: ' || p_day
                || ', p_version: ' || p_version
                || ', p_line: ' || p_line
                || ', p_content: ' || p_content
                );
            --
        end if;
        --
    end load_input;
    --
    function get_input(
        p_year in number,
        p_day in number,
        p_version in number
    ) return input_t is
        --
        t_input input_t;
        --
    begin
        --
        open cs_input(p_year, p_day, p_version);
        fetch cs_input bulk collect into t_input;
        close cs_input;
        --
        return t_input;
        --
    end get_input;
    --
    function get_input return input_t is
        --
        t_input input_t;
        --
    begin
        --
        t_input := get_input(g_year, g_day, g_version);
        --
    end get_input;
    --
end;