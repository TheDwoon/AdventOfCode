-- Test task from 2021 day 01.
-- Part 1: 1557
-- Part 2: 1608
declare
    solution_part1 number;
    solution_part2 number;
begin
    --
    aoc.init(2022, 0, 0);
    aoc.get_input;
    --
    select count(*)
    into solution_part1
    from aoc_input a
             inner join aoc_input b
                        on (a.line + 1 = b.line
                            and TO_NUMBER(a.content) < TO_NUMBER(b.CONTENT));
    --
    aoc.debug('day00', 'p1', solution_part1);
    --
    select count(*) into solution_part2 from (
    with windows as (
        select a.line                                                          as window_start,
            TO_NUMBER(a.content) + TO_NUMBER(b.content) + TO_NUMBER(c.content) as window_value
        from aoc_input a
        inner join aoc_input b on (a.line + 1 = b.line)
        inner join aoc_input c on (b.line + 1 = c.line))
    select a.*
    from windows a
    inner join windows b
    on (a.window_start + 1 = b.window_start
        and a.window_value < b.window_value));
    --
    aoc.debug('day00', 'p2', solution_part2);
    --
    commit;
    --
end;