import { Pool } from "pg";
import "dotenv/config"

const connectionString = process.env.NEON_DB_URL;

if(!connectionString){
    throw new Error("Database URL is not configured!");
}

export const pool = new Pool({
    connectionString
})

