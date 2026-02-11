(define (problem triangle_tire_3)
(:domain triangle-tire)
(:objects
	l1 l2 l3 - location
)
(:init
	(not-flattire)
	(at l1)
	(connected l2 l3)
	(connected l3 l2)
	(connected l1 l2)
	(connected l2 l1)
	(spare-in l1)
	(spare-in l2)
	(spare-in l3)
)
(:goal (and (at l3))))
