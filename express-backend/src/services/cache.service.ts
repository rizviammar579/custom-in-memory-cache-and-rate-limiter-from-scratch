import axios from "axios";
import { pool } from "../db.js";
import type { QueryResult } from "pg";

const secretKey = process.env.CPP_SERVER_SECRET_KEY;

if (!secretKey) {
    throw new Error("CPP Server Secret Key is not configured!");
}

const cacheUrl = "http://127.0.0.1:8081/cache";

enum cacheStatuses {
    HIT = "hit",
    MISS = "miss",
    BYPASS = "bypass",
}



export const cacheService = async (clientIP: string, cacheKey: string, query: string, values: unknown[]) => {

    let dbResult : QueryResult;
    let message: string;


    let cacheStatus: cacheStatuses = cacheStatuses.MISS;

    try {
        const cacheResponse = await axios.get(cacheUrl, {
            params: {
                ip: clientIP,
                key: cacheKey
            },
            headers: {
                "CPP_SERVER_SECRET_KEY": secretKey
            },
            validateStatus: () => true
        })

        if (cacheResponse.status === 429) return {
            status: 429,
            body : {
                message: "Too Many Requests : Rate limit exceeded"
            }
        }

        if (cacheResponse.status === 200) return {
            status: 200,
            body: {
            data: cacheResponse.data,
            cache: cacheStatuses.HIT
            }
        }

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
            query, values
        );

    } catch (error) {

        return {
            status: 500,
            body: {
            message: "Failed to fetch from database",
            cache: cacheStatus
            }
        }

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

        if (cachePopulate.status === 200) message = "Cache Populated after DB query";

        else throw new Error("Cache not populated after DB query");

    } catch (error) {
        message = "Cache not populated"
    }

    return {
        status: 200,
        body: {
        data: dbResult.rows,
        cache: cacheStatus,
        message: message
        }
    }


};