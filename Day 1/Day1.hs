-- Day 1: Secret Entrance
-- https://adventofcode.com/2025/day/1

-- 0x434C49434B = "CLICK"

import Prelude

calculatePassword :: String -> IO Int
calculatePassword path = do
  instructions <- loadInstructions path
  pure $ zeroCount instructions
  where
    zeroCount instructions = length . filter (\(x, _) -> x == 0) $ evaluateClickInstructions start instructions
    start = 50

calculateClickPassword :: String -> IO Int
calculateClickPassword path = do
  instructions <- loadInstructions path
  pure $ zeroCount instructions
  where
    zeroCount instructions = foldr (\(_, clicks) acc -> acc + clicks) 0 $ evaluateClickInstructions start instructions
    start = 50

loadInstructions :: String -> IO [(Char, Int)]
loadInstructions path = do
  rawInstructions <- readFile path
  pure $ parsedInstructions rawInstructions
  where
    parsedInstructions rawInstructions = map (\str -> (head str, read $ tail str)) $ lines rawInstructions

evaluateClickInstructions :: Int -> [(Char, Int)] -> [(Int, Int)]
evaluateClickInstructions _ [] = []
evaluateClickInstructions x ((dir, n):rest) =
  (newX, clicks) : evaluateClickInstructions newX rest
  where
    (newX, clicks) = turnClickDial dir x n

turnClickDial :: Char -> Int ->  Int -> (Int, Int)
turnClickDial dir x dx = case dir of
  'L' -> calculate (-)
  'R' -> calculate (+)
  where
    calculate :: (Int -> Int -> Int) -> (Int, Int)
    calculate sign = (clamp, clicksCount)
      where
        clamp = subtraction `mod` xMax
        clicksCount = abs . floor $ ((fromIntegral subtraction) / 100)
        subtraction = x `sign` dx
        xMax = 100