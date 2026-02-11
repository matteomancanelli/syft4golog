(define (domain robot_coffee)
    (:requirements :strips :non-deterministic)
 
    (:types room)
    (:constants l0 - room)

    (:predicates (robot_at ?x - room)
	    (coffee_at ?x - room)
	    (has_coffee)
        (fairness) 
    )

    (:action move
        :parameters
            (?x - room
            ?y - room)
        :precondition
            (and
            (robot_at ?x))
        :effect
            (and
                (robot_at ?y) (not (robot_at ?x)))
    )

    (:action prepare_coffee
        :parameters
            (?x - room)
        :precondition
            (and
                (= ?x l0) (robot_at ?x) (not (has_coffee)))
        :effect
            (has_coffee)
    )

    (:action put_coffee
        :parameters
            (?x - room)
        :precondition
            (and
                (robot_at ?x) (has_coffee))
        :effect
            (oneof
                (and (coffee_at ?x) (not (has_coffee)))
                (and (fairness))
            )
    )

    (:action put_coffee_fair
        :parameters
            (?x - room)
        :precondition
            (and
                (robot_at ?x) (has_coffee) (fairness))
        :effect
            (and (coffee_at ?x)(not (has_coffee))(not (fairness)))
    )
)