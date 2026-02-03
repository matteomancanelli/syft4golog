(define (domain blocks-table)
  (:requirements :non-deterministic :equality :typing)
  (:types block)
  (:constants b0 - block)
  (:predicates (on ?x - block ?y - block) (clear ?x - block) (holding ?x - block) (emptyhand))

  (:action unstack 
    :parameters (?x ?y - block)
    :precondition (and
      (not (= ?x ?y))
      (emptyhand)
      (on ?x ?y)
      (clear ?x)
    )
    :effect (oneof
      (and
        (holding ?x)
        (not (emptyhand))
        (not (on ?x ?y))
        (clear ?y)
        (not (clear ?x))
      )
      (and
        (on ?x b0)
        (not (on ?x ?y))
        (clear ?x)
        (clear ?y)
      )
    )
  )
  (:action put_down
    :parameters (?x - block)
    :precondition (and (holding ?x) (not (emptyhand)))
    :effect (and
      (on ?x b0)
      (clear ?x)
      (emptyhand)
      (not (holding ?x))
    )
  )
)
