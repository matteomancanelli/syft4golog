(define (domain dishwasher)
  (:requirements :typing :strips :non-deterministic)
  (:types room dish)

  (:predicates
    (can-arrive ?d - dish)
    (dirty-dish-at ?d - dish ?r - room)
    (clean ?d - dish)
    (holding ?d - dish)
  )

  (:action ask
    :parameters (?d - dish)
    :precondition (and (can-arrive ?d))
    :effect (oneof
        (and (dirty-dish-at ?d r1) (not (can-arrive ?d)))
        (and (dirty-dish-at ?d r2) (not (can-arrive ?d)))
        (and (dirty-dish-at ?d r3) (not (can-arrive ?d)))
        (and (dirty-dish-at ?d r4) (not (can-arrive ?d)))
        (and (dirty-dish-at ?d r5) (not (can-arrive ?d)))
      )
  )

  (:action load
    :parameters (?r - room ?d - dish)
    :precondition (and (dirty-dish-at ?d ?r))
    :effect (and 
      (not (dirty-dish-at ?d ?r)) 
      (holding ?d)
    )
  )

  (:action unload
    :parameters (?d - dish)
    :precondition (and (holding ?d))
    :effect (and
      (not (holding ?d))
      (clean ?d)
    )
  )
)
