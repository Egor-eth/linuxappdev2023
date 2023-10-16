set pagination off
b range_get
set $i=0
set $count=35-27
command 1
    printf "@start: %d\n", r->start
    printf "@stop: %d\n", r->stop
    printf "@step: %d\n", r->step
    printf "@_pos: %d\n", r->_pos
    set $i=$i+1
    if ($i>=$count)
        clear
    end
    continue
end

ignore 1 27
run
quit
