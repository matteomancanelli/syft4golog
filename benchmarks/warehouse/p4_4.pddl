(define (problem warehouse_4_4)
(:domain warehouse-domain)
(:objects
	b1 b2 b3 b4 - box
	s1 s2 s3 s4 - shelf
)
(:init
	(emptyhand)
	(robot-at s0)
	(box-at b1 s1)
	(box-at b2 s2)
	(box-at b3 s3)
	(box-at b4 s4)
)
(:goal (and
	(box-at b1 s0)
	(box-at b2 s0)
	(box-at b3 s0)
	(box-at b4 s0)
)))
