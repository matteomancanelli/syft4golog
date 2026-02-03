(define (problem warehouse_2)
(:domain warehouse-domain)
(:objects 
    b0 b1 - box 
    s0 s1 g - shelf)
(:init (emptyhand)(robot-at g)(box-at b0 s0)(box-at b1 s1))
(:goal (and))
)
