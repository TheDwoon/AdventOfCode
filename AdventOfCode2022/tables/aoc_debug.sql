create sequence aoc_debug_s start with 1;
/
create table aoc_debug (
    log_id number not null,
    batch_id number not null,
    package varchar2(128) not null,
    method varchar2(128) not null,
    message varchar2(4000),
    log_level number
)
/
create unique index aoc_debug_u1 on aoc_debug(log_id)
/
create index aoc_debug_n1 on aoc_debug(batch_id)
/