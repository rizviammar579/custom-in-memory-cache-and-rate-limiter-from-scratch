import express from 'express';
import "dotenv/config"
import axios from 'axios';
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

    const clientIP = req.ip;

    if (!clientIP) return res.status(400).json({
        message: "Bad Request: Client IP missing"
    })

    const cacheKey = `${req.method}:${req.originalUrl}`;

    const cacheUrl = "http://127.0.0.1:8081/cache";

    let dbResult;
    let cacheResponse;
    let message;
    
    enum cacheStatuses {
        HIT = "hit",
        MISS = "miss",
        BYPASS = "bypass",
    }

    let cacheStatus: cacheStatuses = cacheStatuses.MISS;

    try {
        cacheResponse = await axios.get(cacheUrl, {
            params: {
                ip: clientIP,
                key: cacheKey
            },
            headers: {
                "CPP_SERVER_SECRET_KEY": secretKey
            },
            validateStatus: () => true
        })

        if (cacheResponse.status === 429) return res.status(429).json({
            message: "Too Many Requests : Rate limit exceeded"
        })

        if (cacheResponse.status == 200) return res.status(200).json({
            data: cacheResponse.data,
            cache: cacheStatuses.HIT
        })

        if (cacheResponse.status !== 404) {
            console.error(
                "Unexpected C++ response:",
                cacheResponse.status
            );
            cacheStatus = cacheStatuses.BYPASS
        }

    }
    catch (error) {
        console.log("C++ cache request failed!");
        cacheStatus = cacheStatuses.BYPASS;
    }

    try {

        dbResult = await pool.query(
            "SELECT * FROM products ORDER BY id"
        );

    } catch (error) {

        return res.status(500).json({
            message: "Failed to fetch from database",
            cache: cacheStatus
        })

    }

    try {

        const cachePopulate = await axios.post(cacheUrl,
            dbResult.rows,
            {
                params: {
                    key: cacheKey
                },
                headers: {
                    "CPP_SERVER_SECRET_KEY": secretKey
                },
                validateStatus: () => true
            })

        if (cachePopulate.status === 200) message = "Cache Populated";

        else throw new Error("Cache not populated");

    } catch (error) {
        message = "Cache not populated"
    }

    return res.status(200).json({
        data: dbResult.rows,
        cache: cacheStatus,
        message: message
    })
  

})


const PORT = 3000;

app.listen(PORT, () => {
    console.log(`Express server running on port ${PORT}`);
});