nat = [1..]

parse content = map read $ lines content

answer input =
    head [
        x * y * z |
        (x, xi) <- zip input nat,
        (y, yi) <- zip (take xi input) nat,
        z <- take yi input,
        x + y + z == 2020
    ]

main = do
    content <- readFile "day1.txt"
    let input = parse content
    putStrLn $ show $ answer input
