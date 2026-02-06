(define (problem warehouse_3)
(:domain warehouse-domain)
(:objects
	b1 b2 b3 - box
	s1 s2 s3 - shelf
)
(:init
	(emptyhand)
	(robot-at s0)
	(box-at b1 s1)
	(box-at b2 s2)
	(box-at b3 s3)
)
(:goal (and
	(box-at b1 s0)
	(box-at b2 s0)
	(box-at b3 s0)
)))
