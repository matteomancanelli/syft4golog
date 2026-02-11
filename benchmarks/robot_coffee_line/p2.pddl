(define (problem robot_coffee_2)
(:domain robot_coffee)
(:objects
	l1 l2 - room
)
(:init
	(robot_at l0)
	(connected l0 l1)
	(connected l1 l0)
	(connected l1 l2)
	(connected l2 l1)
)
(:goal (and (coffee_at l1) (coffee_at l2))))
