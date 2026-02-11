(define (problem robot_coffee_3)
(:domain robot_coffee)
(:objects
	l1 l2 l3 - room
)
(:init
	(robot_at l0)
	(connected l0 l1)
	(connected l1 l0)
	(connected l1 l2)
	(connected l2 l1)
	(connected l2 l3)
	(connected l3 l2)
)
(:goal (and (coffee_at l1) (coffee_at l2) (coffee_at l3))))
