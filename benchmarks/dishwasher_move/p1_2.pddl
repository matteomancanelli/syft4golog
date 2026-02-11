(define (problem dishwasher_1_2)
  (:domain dishwasher)
  (:objects
    d1 d2 - dish
    r1 - room
  )
  (:init
    (robot-at r0)
    (emptyhand)
    (can-arrive d1)
    (can-arrive d2)
  )
  (:goal (and
    (clean d1)
    (clean d2)
  ))
)