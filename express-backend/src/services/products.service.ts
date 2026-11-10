import type { Request, Response } from "express";
import { cacheService } from "./cache.service.js";


export const getAllProducts = async (req: Request, res: Response) => {

    const clientIP = req.ip;

    if (!clientIP) return res.status(400).json({
        message: "Bad Request: Client IP missing"
    })

    const cacheKey = `${req.method}:${req.originalUrl}`;

    const query = "SELECT * FROM products ORDER BY id";

    const result = await cacheService(clientIP, cacheKey, query, []);

    return res.status(result.status).json(result.body);

}

export const getProductById = async (req: Request, res: Response) => {

    const id = Number(req.params.id);

    if (!Number.isInteger(id) || id <= 0) {
        return res.status(400).json({
            message: "Invalid product ID"
        });
    }

    const clientIP = req.ip;

    if (!clientIP) return res.status(400).json({
        message: "Bad Request: Client IP missing"
    })

    const cacheKey = `${req.method}:${req.originalUrl}`;

    const query = `SELECT * FROM products WHERE id = $1`;

    const result = await cacheService(clientIP, cacheKey, query, [id]);

    return res.status(result.status).json(result.body);

}

export const getProductsByPrice = async (req: Request, res: Response) => {

    const price = Number(req.params.price);

    if (!Number.isFinite(price) || price <= 0) {
        return res.status(400).json({
            message: "Invalid price"
        });
    }

    const clientIP = req.ip;

    if (!clientIP) return res.status(400).json({
        message: "Bad Request: Client IP missing"
    })

    const cacheKey = `${req.method}:${req.originalUrl}`;

    const query = `SELECT * FROM products WHERE price <= $1`;

    const result = await cacheService(clientIP, cacheKey, query, [price]);

    return res.status(result.status).json(result.body);

}

export const getProductsByCategory = async (req: Request, res: Response) => {

    const { category } = req.params; 

    const clientIP = req.ip;

    if (!clientIP) return res.status(400).json({
        message: "Bad Request: Client IP missing"
    })

    const cacheKey = `${req.method}:${req.originalUrl}`;

    const query = `SELECT * FROM products WHERE category = $1`;

    const result = await cacheService(clientIP, cacheKey, query, [category]);

    return res.status(result.status).json(result.body);

}