import express from 'express';
import "dotenv/config";
import type { Request, Response } from 'express';
import { pool } from './db.js'
import productRoutes from './routes/products.routes.js'


const app = express();

app.use(express.json());


app.get("/", (req: Request, res: Response) => {
    res.status(200).json({
        message: "Express TypeScript server is running!"
    });
});


app.get("/db-health", async (req: Request, res: Response) => {

    try {

        const result = await pool.query("SELECT NOW()");

        res.status(200).json({
            message: "Neon PostgreSQL connected!",
            time: result.rows[0].now
        });

    } catch (error) {

        console.error("Database connection failed:", error);

        res.status(500).json({
            message: "Database connection failed!"
        });

    }

});


app.use("/products", productRoutes);


const PORT: number = 3000;

app.listen(PORT, () => {
    console.log(`Express server running on port ${PORT}`);
});