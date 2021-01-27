-- This is not as efficient as desired, because the cupMap is not mutable
-- itself, so M.union must copy a bunch of data:-( Perhaps Data.STM.LinkedList
-- would be helpful?

import qualified Data.List as L
import qualified Data.Map as M
import qualified Data.Maybe as Maybe

inputText :: String
inputText = "364297581"

cupMap :: [Int] -> M.Map Int Int
cupMap cups =
    M.fromList $ zip cups (tail cups ++ [head cups])

destCup :: Int -> Int -> [Int] -> Int
destCup maxCup curCup pickupCups =
    if destCup' `elem` pickupCups then
        destCup maxCup destCup' pickupCups
    else
        destCup'
    where
        destCup' = if curCup == 1 then maxCup else curCup - 1

move :: Int -> (Int, M.Map Int Int) -> (Int, M.Map Int Int)
move maxCup (curCup, cupMap) = (curCup', cupMap')
    where
        pickupCup1 = Maybe.fromJust $ M.lookup curCup cupMap
        pickupCup2 = Maybe.fromJust $ M.lookup pickupCup1 cupMap
        pickupCup3 = Maybe.fromJust $ M.lookup pickupCup2 cupMap
        afterPickupCup3 = Maybe.fromJust $ M.lookup pickupCup3 cupMap
        destCup' = destCup maxCup curCup [pickupCup1, pickupCup2, pickupCup3]
        afterDestCup = Maybe.fromJust $ M.lookup destCup' cupMap
        curCup' = afterPickupCup3
        cupMapUpdates =
            M.fromList [
                (curCup, afterPickupCup3),
                (destCup', pickupCup1),
                --(pickupCup1, pickupCup2),
                --(pickupCup2, pickupCup3),
                (pickupCup3, afterDestCup)
            ]
        cupMap' = M.union cupMapUpdates cupMap

answer :: [Int] -> Int
answer cups = answer'
    where
        maxSuppliedCup = maximum cups
        maxCup = 1000000
        fullCups = cups ++ [maxSuppliedCup+1..maxCup]
        initCurCup = head fullCups
        initCupMap = cupMap fullCups
        (_, finalCupMap) = L.foldl' (\s _ -> move maxCup s) (initCurCup, initCupMap) [1..10000000]
        after1 = Maybe.fromJust $ M.lookup 1 finalCupMap
        after1' = Maybe.fromJust $ M.lookup after1 finalCupMap
        answer' = after1 * after1'

main :: IO ()
main = do
    let input = map read $ map (\c -> [c]) inputText
    putStrLn $ show $ answer $ input
