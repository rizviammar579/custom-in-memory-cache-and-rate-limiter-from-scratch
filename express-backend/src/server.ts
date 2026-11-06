import express from 'express';
import "dotenv/config"
import type { Request, Response } from 'express';

const secretKey = process.env.CPP_SERVER_SECRET_KEY;

if (!secretKey) {
    throw new Error("CPP Server Secret Key is not configured!");
}

const app = express();

app.use(express.json());

interface Product {
    id: number;
    name: string;
    price: number;
}

const products: Product[] = [
    { id: 1, name: "Laptop", price: 55000 },
    { id: 2, name: "Keyboard", price: 1500 },
    { id: 3, name: "Mouse", price: 800 }
];

app.get("/", (req: Request, res: Response) => {
    res.status(200).json({
        message: "Express TypeScript server is running!"
    });
})

app.get("/api/products", (req: Request, res: Response) => {
    res.status(200).json(products);
})

const PORT = 3000;

app.listen(PORT, () => {
    console.log(`Express server running on port ${PORT}`);
});