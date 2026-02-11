(define (problem dishwasher_3_3)
  (:domain dishwasher)
  (:objects
    d1 d2 d3 - dish
    r1 r2 r3 - room
  )
  (:init
    (robot-at r0)
    (emptyhand)
    (can-arrive d1)
    (can-arrive d2)
    (can-arrive d3)
  )
  (:goal (and
    (clean d1)
    (clean d2)
    (clean d3)
  ))
)