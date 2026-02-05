(define (problem warehouse_2_2)
(:domain warehouse-domain)
(:objects
	b1 b2 - box
	s1 s2 - shelf
)
(:init
	(emptyhand)
	(robot-at s0)
	(box-at b1 s1)
	(box-at b2 s2)
)
(:goal (and
	(box-at b1 s0)
	(box-at b2 s0)
)))
