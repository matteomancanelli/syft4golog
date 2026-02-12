(define (problem dishwasher_r2_d6)
(:domain warehouse-domain)
(:objects
  d1 d2 d3 d4 d5 d6 - dish
  r1 r2 - room
)
(:init
  (emptyhand)
  (robot-at r0)
  (dish-at d1 r1)
  (dish-at d2 r2)
  (dish-at d3 r1)
  (dish-at d4 r2)
  (dish-at d5 r1)
  (dish-at d6 r2)
)
(:goal (and
  (dish-at d1 r0)
  (dish-at d2 r0)
  (dish-at d3 r0)
  (dish-at d4 r0)
  (dish-at d5 r0)
  (dish-at d6 r0)
))
)