import express from 'express';
import "dotenv/config"
import type { Request, Response } from 'express';
import { pool } from './db.js'

const secretKey = process.env.CPP_SERVER_SECRET_KEY;

if (!secretKey) {
    throw new Error("CPP Server Secret Key is not configured!");
}

const app = express();

app.use(express.json());


app.get("/", (req: Request, res: Response) => {
    res.status(200).json({
        message: "Express TypeScript server is running!"
    });
})

app.get("/db-health", async (req: Request, res: Response) => {

    try {
        const result = await pool.query("SELECT NOW()");

        res.status(200).json({
            message: "Neon PostgreSQL connected!",
            time: result.rows[0].now
        })
    } catch (error) {

        console.error("Database connection failed:", error);

        res.status(500).json({
            message: "Database connection failed!"
        })

    }

})

app.get("/api/products", async (req: Request, res: Response) => {

    try {
        const result = await pool.query(
            "SELECT * FROM products ORDER BY id"
        );

        res.status(200).json(result.rows)

    } catch (error) {

        console.error("Database query failed:", error);

        res.status(500).json({
            message: "Failed to fetch products"
        })

    }

})

const PORT = 3000;

app.listen(PORT, () => {
    console.log(`Express server running on port ${PORT}`);
});