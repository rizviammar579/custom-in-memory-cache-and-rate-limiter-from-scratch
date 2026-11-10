import { Router } from "express";
import {
    getAllProducts,
    getProductById,
    getProductsByCategory,
    getProductsByPrice
} from "../services/products.service.js"


const router = Router();

router.get('/', getAllProducts);
router.get('/category/:category', getProductsByCategory);
router.get('/price/:price', getProductsByPrice);
router.get('/:id', getProductById);

export default router;