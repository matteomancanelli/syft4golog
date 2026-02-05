(define (problem warehouse_5_5)
(:domain warehouse-domain)
(:objects
	b1 b2 b3 b4 b5 - box
	s1 s2 s3 s4 s5 - shelf
)
(:init
	(emptyhand)
	(robot-at s0)
	(box-at b1 s1)
	(box-at b2 s2)
	(box-at b3 s3)
	(box-at b4 s4)
	(box-at b5 s5)
)
(:goal (and
	(box-at b1 s0)
	(box-at b2 s0)
	(box-at b3 s0)
	(box-at b4 s0)
	(box-at b5 s0)
)))
