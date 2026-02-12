(define (problem dishwasher_r3_d7)
(:domain warehouse-domain)
(:objects
  d1 d2 d3 d4 d5 d6 d7 - dish
  r1 r2 r3 - room
)
(:init
  (emptyhand)
  (robot-at r0)
  (dish-at d1 r1)
  (dish-at d2 r2)
  (dish-at d3 r3)
  (dish-at d4 r1)
  (dish-at d5 r2)
  (dish-at d6 r3)
  (dish-at d7 r1)
)
(:goal (and
  (dish-at d1 r0)
  (dish-at d2 r0)
  (dish-at d3 r0)
  (dish-at d4 r0)
  (dish-at d5 r0)
  (dish-at d6 r0)
  (dish-at d7 r0)
))
)