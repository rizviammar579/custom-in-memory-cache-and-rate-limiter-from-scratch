import express from 'express';
import type { Request, Response } from 'express';

const app = express();

app.use(express.json());

app.get("/", (req: Request, res: Response) => {
    res.status(200).json({
        message: "Express TypeScript server is running!"
    });
})

const PORT = 3000;

app.listen(PORT, () => {
    console.log(`Express server running on port ${PORT}`);
});