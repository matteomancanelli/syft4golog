(define (problem dishwasher_3_4)
  (:domain dishwasher)
  (:objects
    d1 d2 d3 d4 - dish
    r1 r2 r3 - room
  )
  (:init
    (robot-at r0)
    (emptyhand)
    (can-arrive d1)
    (can-arrive d2)
    (can-arrive d3)
    (can-arrive d4)
  )
  (:goal (and
    (clean d1)
    (clean d2)
    (clean d3)
    (clean d4)
  ))
)