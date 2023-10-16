set pagination off
b range_get if (r->_pos % 5) == 0
command 1
    printf "@start: %d\n", r->start
    printf "@stop: %d\n", r->stop
    printf "@step: %d\n", r->step
    printf "@_pos: %d\n", r->_pos
    continue
end

run
quit
